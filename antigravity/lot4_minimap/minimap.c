/*
 * lot4_minimap/minimap.c
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 4 — Minimap + advanced collisions + save/load
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL_image.h>

#include "minimap.h"
#include "../lot2_background/background.h"

extern Background *backg;

/* ------------------------------------------------------------------ */
/*  Variables minimap                                                 */
/* ------------------------------------------------------------------ */
static SDL_Rect miniMapRect = {0, 0, 160, 90};
static float scaleX = 1.0f;
static float scaleY = 1.0f;
static Joueur *joueur2MiniMap = NULL;
static SDL_Texture *miniMapTexture = NULL;

/* ------------------------------------------------------------------ */
/*  Animation collision                                               */
/* ------------------------------------------------------------------ */
typedef struct
{
    int frame;
    int active;
    int tick;
    SDL_Rect pos;
} AnimationCollision;

static AnimationCollision animCollision = {0, 0, 0, {0, 0, 32, 32}};

/* ------------------------------------------------------------------ */
/*  Sous-menu sauvegarde                                              */
/* ------------------------------------------------------------------ */
static SDL_Rect btnSave = {0, 0, 180, 50};
static SDL_Rect btnLoad = {0, 0, 180, 50};
static int messageAffiche = 0;

/* ------------------------------------------------------------------ */
/*  Initialisation                                                    */
/* ------------------------------------------------------------------ */
void initMiniMap(SDL_Renderer *ren)
{
    int w = 1280;
    int h = 720;
    SDL_Surface *tmp = NULL;

    if (ren)
        SDL_GetRendererOutputSize(ren, &w, &h);

    miniMapRect.w = 160;
    miniMapRect.h = 90;
    miniMapRect.x = w - miniMapRect.w - 20;
    miniMapRect.y = h - miniMapRect.h - 20;

    if (backg && backg->mapW > 0 && backg->mapH > 0)
    {
        scaleX = 160.0f / backg->mapW;
        scaleY = 90.0f / backg->mapH;
    }
    else
    {
        scaleX = 1.0f;
        scaleY = 1.0f;
    }

    if (!miniMapTexture && ren)
    {
        tmp = IMG_Load("minimap.jpg");
        if (tmp)
        {
            miniMapTexture = SDL_CreateTextureFromSurface(ren, tmp);
            SDL_FreeSurface(tmp);
            tmp = NULL;
        }
        else
        {
            printf("Erreur chargement minimap.jpg : %s\n", IMG_GetError());
        }
    }
}

void initSousMenuSauvegarde(SDL_Renderer *ren)
{
    int w = 1280;
    int h = 720;

    if (ren)
        SDL_GetRendererOutputSize(ren, &w, &h);

    btnSave.x = w / 2 - 90;
    btnSave.y = h / 2 - 60;

    btnLoad.x = w / 2 - 90;
    btnLoad.y = h / 2 + 10;
}

/* ------------------------------------------------------------------ */
/*  Affichage                                                         */
/* ------------------------------------------------------------------ */
void afficherMiniMap(SDL_Renderer *ren, Joueur *j,
                     NPC *ennemis, int nbEnnemis)
{
    int i;

    if (!ren || !j || !backg)
        return;

    if (backg->mapW > 0 && backg->mapH > 0)
    {
        scaleX = 160.0f / backg->mapW;
        scaleY = 90.0f / backg->mapH;
    }

    /* fond minimap avec image */
    if (miniMapTexture)
    {
        SDL_RenderCopy(ren, miniMapTexture, NULL, &miniMapRect);
    }
    else
    {
        SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
        SDL_RenderFillRect(ren, &miniMapRect);
    }

    /* bordure */
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &miniMapRect);

    /* joueur principal : blanc */
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_Rect dotPlayer = {
        miniMapRect.x + (int)(j->rect.x * scaleX),
        miniMapRect.y + (int)(j->rect.y * scaleY),
        4, 4
    };
    SDL_RenderFillRect(ren, &dotPlayer);

    /* deuxième joueur : jaune */
    if (joueur2MiniMap)
    {
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_Rect dotPlayer2 = {
            miniMapRect.x + (int)(joueur2MiniMap->rect.x * scaleX),
            miniMapRect.y + (int)(joueur2MiniMap->rect.y * scaleY),
            4, 4
        };
        SDL_RenderFillRect(ren, &dotPlayer2);
    }

    /* ennemis : rouge */
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    for (i = 0; i < nbEnnemis; i++)
    {
        if (ennemis[i].active)
        {
            SDL_Rect dotEnemy = {
                miniMapRect.x + (int)(ennemis[i].rect.x * scaleX),
                miniMapRect.y + (int)(ennemis[i].rect.y * scaleY),
                3, 3
            };
            SDL_RenderFillRect(ren, &dotEnemy);
        }
    }

    /* viewport caméra : vert */
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_Rect viewport = {
        miniMapRect.x + (int)(backg->offsetX * scaleX),
        miniMapRect.y + (int)(backg->offsetY * scaleY),
        (int)(640 * scaleX),
        (int)(360 * scaleY)
    };
    SDL_RenderDrawRect(ren, &viewport);

    /* animation collision */
    if (animCollision.active)
    {
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);

        SDL_Rect r = {
            miniMapRect.x + (int)(animCollision.pos.x * scaleX),
            miniMapRect.y + (int)(animCollision.pos.y * scaleY),
            4 + animCollision.frame * 2,
            4 + animCollision.frame * 2
        };

        SDL_RenderDrawRect(ren, &r);

        animCollision.tick++;
        if (animCollision.tick % 3 == 0)
            animCollision.frame++;

        if (animCollision.frame > 5)
        {
            animCollision.active = 0;
            animCollision.frame = 0;
            animCollision.tick = 0;
        }
    }
}

void afficherSousMenuSauvegarde(SDL_Renderer *ren)
{
    if (!ren)
        return;

    SDL_Rect panel = {
        btnSave.x - 20,
        btnSave.y - 20,
        220,
        130
    };

    SDL_SetRenderDrawColor(ren, 25, 25, 25, 255);
    SDL_RenderFillRect(ren, &panel);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &panel);

    SDL_SetRenderDrawColor(ren, 0, 120, 255, 255);
    SDL_RenderFillRect(ren, &btnSave);

    SDL_SetRenderDrawColor(ren, 0, 180, 100, 255);
    SDL_RenderFillRect(ren, &btnLoad);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &btnSave);
    SDL_RenderDrawRect(ren, &btnLoad);

    if (messageAffiche)
    {
        printf("Jeu sauvegardé / chargé !\n");
        messageAffiche = 0;
    }
}

/* ------------------------------------------------------------------ */
/*  Update                                                            */
/* ------------------------------------------------------------------ */
void majMiniMap(Joueur *j1, Joueur *j2)
{
    (void)j1;
    joueur2MiniMap = j2;
}

/* ------------------------------------------------------------------ */
/*  Collisions                                                        */
/* ------------------------------------------------------------------ */
int collisionPixelParfait(SDL_Texture *bg, SDL_Rect *rect,
                          int offsetX, int offsetY)
{
    (void)bg;
    (void)rect;
    (void)offsetX;
    (void)offsetY;

    /* Version simple pour éviter l'erreur SDL_Texture / SDL_Surface */
    return 0;
}

int collisionPlatformeMobile(SDL_Rect *joueur, Plateforme *p)
{
    if (!joueur || !p || !p->active)
        return 0;

    if (!detectionBB(joueur, &p->rect))
        return 0;

    /* collision par dessus */
    if ((joueur->y + joueur->h) <= (p->rect.y + 15))
    {
        joueur->y = p->rect.y - joueur->h;
        return 1;
    }

    /* collision côté */
    joueur->x -= p->dx;
    return 1;
}

void animerCollisionBackground(SDL_Renderer *ren, int x, int y)
{
    (void)ren;

    animCollision.active = 1;
    animCollision.frame = 0;
    animCollision.tick = 0;
    animCollision.pos.x = x;
    animCollision.pos.y = y;
    animCollision.pos.w = 32;
    animCollision.pos.h = 32;
}

/* ------------------------------------------------------------------ */
/*  Nettoyage                                                         */
/* ------------------------------------------------------------------ */
void freeMiniMap(void)
{
    if (miniMapTexture)
    {
        SDL_DestroyTexture(miniMapTexture);
        miniMapTexture = NULL;
    }
}
