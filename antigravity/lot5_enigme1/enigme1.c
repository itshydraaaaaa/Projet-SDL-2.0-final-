#include "enigme1.h"

static Question questions[MAX_QUESTIONS];
static int nbQuestions = 0;

void initEnigme1(const char *fichier) {
    nbQuestions = 0;
    if (!fichier) return;

    FILE *f = fopen(fichier, "r");
    if (!f) { 
        fprintf(stderr, "[enigme1] Cannot open %s\n", fichier); 
        return; 
    }

    char line[512];
    while (nbQuestions < MAX_QUESTIONS && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        char *tok = strtok(line, "|");
        if (!tok) continue;
        strncpy(questions[nbQuestions].question, tok, 255);
        questions[nbQuestions].question[255] = '\0';

        for (int i = 0; i < 3; i++) {
            tok = strtok(NULL, "|");
            if (!tok) break;
            strncpy(questions[nbQuestions].choix[i], tok, 127);
            questions[nbQuestions].choix[i][127] = '\0';
        }
        tok = strtok(NULL, "|");
        questions[nbQuestions].bonneReponse = tok ? atoi(tok) : 1;
        questions[nbQuestions].dejaVue = 0;
        questions[nbQuestions].texture_question = NULL;
        questions[nbQuestions].texture_reponse1 = NULL;
        questions[nbQuestions].texture_reponse2 = NULL;
        questions[nbQuestions].texture_reponse3 = NULL;
        questions[nbQuestions].etat = 0;
        questions[nbQuestions].temps_limite = 30;
        
        nbQuestions++;
    }
    fclose(f);
    printf("[enigme1] Loaded %d questions\n", nbQuestions);
}

int getNbQuestions(void) {
    return nbQuestions;
}

Question* getQuestions(void) {
    return questions;
}

void initialiserTexturesEnigme(SDL_Renderer *renderer, Question *q, TTF_Font *font) {
    if (!renderer || !q || !font) return;
    
    SDL_Surface *surface;
    SDL_Color couleurQuestion = {255, 255, 255, 255};
    SDL_Color couleurReponses = {255, 215, 0, 255};
    
    libererTexturesEnigme(q);
    
    surface = TTF_RenderText_Blended_Wrapped(font, q->question, couleurQuestion, 700);
    if (surface) {
        q->texture_question = SDL_CreateTextureFromSurface(renderer, surface);
        q->rect_question = (SDL_Rect){(SCREEN_WIDTH - surface->w) / 2, 120, surface->w, surface->h};
        SDL_FreeSurface(surface);
    }
    
    char rep1_text[256];
    sprintf(rep1_text, "1. %s", q->choix[0]);
    surface = TTF_RenderText_Blended(font, rep1_text, couleurReponses);
    if (surface) {
        q->texture_reponse1 = SDL_CreateTextureFromSurface(renderer, surface);
        q->rect_reponse1 = (SDL_Rect){(SCREEN_WIDTH - surface->w) / 2, 260, surface->w, surface->h};
        SDL_FreeSurface(surface);
    }
    
    char rep2_text[256];
    sprintf(rep2_text, "2. %s", q->choix[1]);
    surface = TTF_RenderText_Blended(font, rep2_text, couleurReponses);
    if (surface) {
        q->texture_reponse2 = SDL_CreateTextureFromSurface(renderer, surface);
        q->rect_reponse2 = (SDL_Rect){(SCREEN_WIDTH - surface->w) / 2, 340, surface->w, surface->h};
        SDL_FreeSurface(surface);
    }
    
    char rep3_text[256];
    sprintf(rep3_text, "3. %s", q->choix[2]);
    surface = TTF_RenderText_Blended(font, rep3_text, couleurReponses);
    if (surface) {
        q->texture_reponse3 = SDL_CreateTextureFromSurface(renderer, surface);
        q->rect_reponse3 = (SDL_Rect){(SCREEN_WIDTH - surface->w) / 2, 420, surface->w, surface->h};
        SDL_FreeSurface(surface);
    }
}

void afficherEnigme1(SDL_Renderer *ren, TTF_Font *font, Question *q) {
    if (!ren || !q) return;
    
    if (!q->texture_question && font) {
        initialiserTexturesEnigme(ren, q, font);
    }
    
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(ren, &overlay);
    
    if (q->texture_question) {
        SDL_RenderCopy(ren, q->texture_question, NULL, &q->rect_question);
    }
    if (q->texture_reponse1) {
        SDL_RenderCopy(ren, q->texture_reponse1, NULL, &q->rect_reponse1);
    }
    if (q->texture_reponse2) {
        SDL_RenderCopy(ren, q->texture_reponse2, NULL, &q->rect_reponse2);
    }
    if (q->texture_reponse3) {
        SDL_RenderCopy(ren, q->texture_reponse3, NULL, &q->rect_reponse3);
    }
}

void afficherChronometre(SDL_Renderer *ren, Uint32 startTime, int duree) {
    if (!ren) return;
    
    Uint32 tempsActuel = SDL_GetTicks();
    int tempsEcoule = (tempsActuel - startTime) / 1000;
    int tempsRestant = duree - tempsEcoule;
    if (tempsRestant < 0) tempsRestant = 0;
    
    float ratio = (float)tempsRestant / duree;
    
    if (tempsRestant > 20) {
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    } else if (tempsRestant > 10) {
        SDL_SetRenderDrawColor(ren, 255, 165, 0, 255);
    } else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    }
    
    int barWidth = (int)((SCREEN_WIDTH - 100) * ratio);
    SDL_Rect timerBar = {50, 20, barWidth, 20};
    SDL_RenderFillRect(ren, &timerBar);
    
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_Rect timerBorder = {48, 18, SCREEN_WIDTH - 96, 24};
    SDL_RenderDrawRect(ren, &timerBorder);
}

Question *genererQuestion(Question *qs, int nb) {
    if (!qs || nb <= 0) return NULL;

    int allSeen = 1;
    for (int i = 0; i < nb; i++)
        if (!qs[i].dejaVue) { allSeen = 0; break; }
    if (allSeen)
        for (int i = 0; i < nb; i++) qs[i].dejaVue = 0;

    int idx;
    int attempts = 0;
    do { 
        idx = rand() % nb; 
        attempts++;
        if (attempts > 100) break;
    } while (qs[idx].dejaVue);
    
    qs[idx].dejaVue = 1;
    qs[idx].etat = 0;
    qs[idx].temps_debut = SDL_GetTicks();
    qs[idx].temps_limite = 30;
    qs[idx].temps_ecoule = 0;
    
    return &qs[idx];
}

void resolveEnigme1(Joueur *j, int reponseDonnee, int bonneReponse) {
    if (!j) return;

    if (reponseDonnee == bonneReponse) {
        ajouterScore(j, 50);
        j->vie++;
        if (j->vie > 5) j->vie = 5;
        if (gameBg.limiteSec > 0) {
            gameBg.limiteSec += 30;
        }
        printf("[enigme1] Correct! +50 pts, +1 vie\n");
    } else {
        perdreVie(j);
        if (gameBg.limiteSec > 0) {
            gameBg.limiteSec -= 10;
            if (gameBg.limiteSec < 0) gameBg.limiteSec = 0;
        }
        printf("[enigme1] Wrong answer! -1 vie\n");
    }
}

int gererClicEnigme(Question *q, int x, int y) {
    if (!q || q->etat != 0) return 0;
    
    if (x >= q->rect_reponse1.x && x <= q->rect_reponse1.x + q->rect_reponse1.w &&
        y >= q->rect_reponse1.y && y <= q->rect_reponse1.y + q->rect_reponse1.h) {
        return (q->bonneReponse == 1) ? 1 : -1;
    }
    else if (x >= q->rect_reponse2.x && x <= q->rect_reponse2.x + q->rect_reponse2.w &&
             y >= q->rect_reponse2.y && y <= q->rect_reponse2.y + q->rect_reponse2.h) {
        return (q->bonneReponse == 2) ? 1 : -1;
    }
    else if (x >= q->rect_reponse3.x && x <= q->rect_reponse3.x + q->rect_reponse3.w &&
             y >= q->rect_reponse3.y && y <= q->rect_reponse3.y + q->rect_reponse3.h) {
        return (q->bonneReponse == 3) ? 1 : -1;
    }
    
    return 0;
}

void mettreAJourTempsEnigme(Question *q) {
    if (!q || q->etat != 0) return;
    
    Uint32 tempsActuel = SDL_GetTicks();
    q->temps_ecoule = (tempsActuel - q->temps_debut) / 1000;
    
    if (q->temps_ecoule >= q->temps_limite && q->etat == 0) {
        q->etat = -1;
    }
}

void libererTexturesEnigme(Question *q) {
    if (!q) return;
    
    if (q->texture_question) {
        SDL_DestroyTexture(q->texture_question);
        q->texture_question = NULL;
    }
    if (q->texture_reponse1) {
        SDL_DestroyTexture(q->texture_reponse1);
        q->texture_reponse1 = NULL;
    }
    if (q->texture_reponse2) {
        SDL_DestroyTexture(q->texture_reponse2);
        q->texture_reponse2 = NULL;
    }
    if (q->texture_reponse3) {
        SDL_DestroyTexture(q->texture_reponse3);
        q->texture_reponse3 = NULL;
    }
}

void afficherMessageReponse(SDL_Renderer *renderer, int succes, Uint32 duree_ms, TTF_Font *font) {
    if (!renderer || !font) return;
    
    SDL_Color couleur;
    const char *message;
    
    if (succes) {
        couleur = (SDL_Color){0, 255, 0, 255};
        message = "BRAVO! +50 points! +1 vie!";
    } else {
        couleur = (SDL_Color){255, 0, 0, 255};
        message = "RATE! -1 vie!";
    }
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, message, couleur);
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {SCREEN_WIDTH/2 - surface->w/2, SCREEN_HEIGHT/2 - surface->h/2, 
                         surface->w, surface->h};
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect fond = {rect.x - 10, rect.y - 10, rect.w + 20, rect.h + 20};
        SDL_RenderFillRect(renderer, &fond);
        
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        if (duree_ms > 0) SDL_Delay(duree_ms);
        
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}
