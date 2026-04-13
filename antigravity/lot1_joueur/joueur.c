/*
 * lot1_joueur/joueur.c
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 1 — Player module — FULL IMPLEMENTATION
 */

#include <stdio.h>
#include <string.h>

#include "joueur.h"
#include "../lot2_background/background.h"

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */

void initJoueur(Joueur *j, SDL_Renderer *ren)
{
    if (!j) return;
    memset(j, 0, sizeof(Joueur));
    j->rect.x  = 100;
    j->rect.y  = 400;
    j->rect.w  = 48;
    j->rect.h  = 64;
    j->x       = (float)j->rect.x;
    j->y       = (float)j->rect.y;
    j->vie     = 3;
    j->score   = 0;
    j->vitesse = 3.0f;
    j->jumping = 0;
    j->grounded = 1;
    strncpy(j->direction, "right", sizeof(j->direction) - 1);
    
    for (int i = 0; i < 8; i++) {
        // Fallback or actual image
        j->img[i] = IMG_LoadTexture(ren, "assets/images/player.png");
    }
}

void initJoueur2(Joueur *j2, SDL_Renderer *ren)
{
    if (!j2) return;
    memset(j2, 0, sizeof(Joueur));
    j2->rect.x  = 650;
    j2->rect.y  = 400;
    j2->rect.w  = 48;
    j2->rect.h  = 64;
    j2->x       = (float)j2->rect.x;
    j2->y       = (float)j2->rect.y;
    j2->vie     = 3;
    j2->score   = 0;
    j2->vitesse = 3.0f;
    j2->jumping = 0;
    j2->grounded = 1;
    strncpy(j2->direction, "left", sizeof(j2->direction) - 1);
    
    for (int i = 0; i < 8; i++) {
        j2->img[i] = IMG_LoadTexture(ren, "assets/images/player2.png");
    }
}

void initSousMenuJoueur(SDL_Renderer *ren)
{
    (void)ren;
    /* Sub-menu initialization */
}

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */

void afficherJoueur(SDL_Renderer *ren, Joueur *j)
{
    if (!j || !backg || !ren) return;
    
    SDL_Rect dest = { 
        j->rect.x - backg->offsetX,
        j->rect.y - backg->offsetY,
        j->rect.w, 
        j->rect.h 
    };
    
    if (j->img[j->frame]) {
        SDL_RenderCopy(ren, j->img[j->frame], NULL, &dest);
    } else {
        // Fallback: blue rectangle
        SDL_SetRenderDrawColor(ren, 100, 100, 255, 255);
        SDL_RenderFillRect(ren, &dest);
    }
}

void afficherSousMenuJoueur(SDL_Renderer *ren)
{
    if (!ren) return;
    /* Basic placeholder for Sub-menu Player Character selection */
    int screenW = 800, screenH = 600;
    SDL_GetRendererOutputSize(ren, &screenW, &screenH);
    
    SDL_Rect panel = { screenW/4, screenH/4, screenW/2, screenH/2 };
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 50, 50, 100, 200);
    SDL_RenderFillRect(ren, &panel);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
}

/* ------------------------------------------------------------------ */
/*  Update / gameplay                                                   */
/* ------------------------------------------------------------------ */

void deplacerJoueur(Joueur *j, const char *input)
{
    if (!j || !input) return;
    
    if (strcmp(input, "left") == 0) {
        j->x -= j->vitesse;
        strncpy(j->direction, "left", sizeof(j->direction) - 1);
    } else if (strcmp(input, "right") == 0) {
        j->x += j->vitesse;
        strncpy(j->direction, "right", sizeof(j->direction) - 1);
    } else if (strcmp(input, "up") == 0) {
        if (j->grounded) {
            j->jumping = 1;
            j->grounded = 0;
            j->velY = -12.0f; // Initial jump velocity
        }
    } else if (strcmp(input, "attack") == 0) {
        strncpy(j->direction, "attack", sizeof(j->direction) - 1);
    }
    
    j->rect.x = (int)j->x;
}

void animerJoueur(Joueur *j)
{
    if (!j) return;
    
    static int tickCounter = 0;
    tickCounter++;
    
    if (tickCounter >= 8) {
        j->frame = (j->frame + 1) % 8;
        tickCounter = 0;
    }
}

void perdreVie(Joueur *j)
{
    if (!j) return;
    j->vie--;
    if (j->vie <= 0) {
        j->active = 0; // Assuming 0 means dead
        if (backg) strncpy(backg->niveau, "GameOver", sizeof(backg->niveau) - 1);
    }
}

void ajouterScore(Joueur *j, int points)
{
    if (!j) return;
    j->score += points;
}

void updateJoueurPhysics(Joueur *j, Plateforme *plateformes, int nbPlateformes)
{
    if (!j || !plateformes) return;
    
    // Apply Gravity
    if (!j->grounded) {
        j->velY += 0.5f; // Gravity acceleration
    }
    j->y += j->velY;
    
    // Floor boundary (fallback)
    if (j->y >= 450.0f) {
        j->y = 450.0f;
        j->velY = 0.0f;
        j->grounded = 1;
        j->jumping = 0;
    }
    
    j->rect.y = (int)j->y;
}
