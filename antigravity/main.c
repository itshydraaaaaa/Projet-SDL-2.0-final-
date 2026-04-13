#include "common.h"
#include "lot1_joueur/joueur.h"
#include "lot2_background/background.h"
#include "lot3_npc/npc.h"
#include "lot4_minimap/minimap.h"
#include "lot5_enigme1/enigme1.h"


int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    srand(time(NULL));
    
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    
    if (TTF_Init() != 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("IMG_Init error: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Antigravity - Home Alone Quiz",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("Window error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Load font (optional - game works without it)
    TTF_Font* font = TTF_OpenFont("assets/fonts/game_font.ttf", 24);
    if (!font) {
        printf("Font not found - text will not display\n");
    }
    
    // Init modules
    initBackground(renderer, "assets/images/bg_level1.png", "assets/images/bg_level2.png");
    initTimer();
    initGuide(renderer, font);
    initSousMenuScores(renderer, font);
    initMiniMap(renderer);
    initEnigme1("lot5_enigme1/questions.txt");
    
    // Init player
    Joueur joueur, joueur2;
    initJoueur(&joueur, renderer);
    initJoueur2(&joueur2, renderer);
    
    // Init enemies
    NPC ennemis[3];
    initNPC(&ennemis[0], renderer, 300, 400);
    initNPC(&ennemis[1], renderer, 500, 400);
    initNPC(&ennemis[2], renderer, 700, 400);
    
    // Init platforms
    Plateforme plateformes[10];
    memset(plateformes, 0, sizeof(plateformes));
    initPlateforme(&plateformes[0], renderer, NULL, 0, 550, SCREEN_WIDTH, 20, FIXE, 0, 0, 0, 0);
    initPlateforme(&plateformes[1], renderer, NULL, 200, 450, 100, 20, FIXE, 0, 0, 0, 0);
    initPlateforme(&plateformes[2], renderer, NULL, 400, 350, 100, 20, MOBILE, 2, 0, 300, 600);
    initPlateforme(&plateformes[3], renderer, NULL, 600, 450, 100, 20, DESTRUCTIBLE, 0, 0, 0, 0);
    
    // Quiz state
    int enigmeActive = 0;
    int showResult = 0;
    int resultSuccess = 0;
    Uint32 resultStartTime = 0;
    Question* currentQuestion = NULL;
    
    int running = 1;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) running = 0;
                if (event.key.keysym.sym == SDLK_q && !enigmeActive) {
                    currentQuestion = genererQuestion(getQuestions(), getNbQuestions());
                    if (currentQuestion) {
                        enigmeActive = 1;
                        if (backg) backg->niveau[0] = 'E';
                    }
                }
                if (!enigmeActive) {
                    if (event.key.keysym.sym == SDLK_LEFT) deplacerJoueur(&joueur, "left");
                    if (event.key.keysym.sym == SDLK_RIGHT) deplacerJoueur(&joueur, "right");
                    if (event.key.keysym.sym == SDLK_UP) deplacerJoueur(&joueur, "up");
                }
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN && enigmeActive && !showResult && currentQuestion) {
                int result = gererClicEnigme(currentQuestion, event.button.x, event.button.y);
                if (result != 0) {
                    resultSuccess = (result == 1);
                    resolveEnigme1(&joueur, result, currentQuestion->bonneReponse);
                    showResult = 1;
                    resultStartTime = SDL_GetTicks();
                }
            }
        }
        
        // Update game state
        if (!enigmeActive) {
            updateScrolling((int)joueur.x, (int)joueur.y, SCREEN_WIDTH, SCREEN_HEIGHT);
            updateTimer();
            updatePlateformesMobiles(plateformes, 10);
            updateJoueurPhysics(&joueur, plateformes, 10);
            
            for (int i = 0; i < 3; i++) {
                if (ennemis[i].active) {
                    deplacerNPCAI(&ennemis[i], &joueur);
                    if (detectionBB(&joueur.rect, &ennemis[i].rect)) {
                        perdreVie(&joueur);
                        ennemis[i].active = 0;
                    }
                }
            }
        }
        
        // Update quiz timer
        if (enigmeActive && currentQuestion && !showResult) {
            mettreAJourTempsEnigme(currentQuestion);
            if (currentQuestion->etat == -1) {
                resultSuccess = 0;
                resolveEnigme1(&joueur, 0, currentQuestion->bonneReponse);
                showResult = 1;
                resultStartTime = SDL_GetTicks();
            }
        }
        
        // Handle result timeout
        if (showResult && SDL_GetTicks() - resultStartTime > 2000) {
            showResult = 0;
            enigmeActive = 0;
            if (currentQuestion) {
                libererTexturesEnigme(currentQuestion);
                currentQuestion = NULL;
            }
        }
        
        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        if (enigmeActive && currentQuestion) {
            afficherBackground(renderer, 1);
            afficherChronometre(renderer, currentQuestion->temps_debut, currentQuestion->temps_limite);
            afficherEnigme1(renderer, font, currentQuestion);
            
            // Show HUD
            char hud[100];
            sprintf(hud, "Score: %d  Lives: %d", joueur.score, joueur.vie);
            if (font) {
                SDL_Surface* surf = TTF_RenderText_Blended(font, hud, (SDL_Color){255,255,255,255});
                if (surf) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                    SDL_RenderCopy(renderer, tex, NULL, &(SDL_Rect){20, 60, surf->w, surf->h});
                    SDL_DestroyTexture(tex);
                    SDL_FreeSurface(surf);
                }
            }
        } else {
            afficherBackground(renderer, 1);
            afficherPlateformes(renderer, plateformes, 10);
            afficherNPC(renderer, ennemis, 3);
            afficherJoueur(renderer, &joueur);
            afficherJoueur(renderer, &joueur2);
            afficherTemps(renderer, font);
            afficherMiniMap(renderer, &joueur, ennemis, 3);
            
            char hud[100];
            sprintf(hud, "Score: %d  Lives: %d  Press Q for Quiz", joueur.score, joueur.vie);
            if (font) {
                SDL_Surface* surf = TTF_RenderText_Blended(font, hud, (SDL_Color){255,255,255,255});
                if (surf) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                    SDL_RenderCopy(renderer, tex, NULL, &(SDL_Rect){10, 10, surf->w, surf->h});
                    SDL_DestroyTexture(tex);
                    SDL_FreeSurface(surf);
                }
            }
        }
        
        if (showResult) {
            afficherMessageReponse(renderer, resultSuccess, 0, font);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    // Cleanup
    libererPlateformes(plateformes, 10);
    libererBackground();
    libererGuide();
    if (font) TTF_CloseFont(font);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
