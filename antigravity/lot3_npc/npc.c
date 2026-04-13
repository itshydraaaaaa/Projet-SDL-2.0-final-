/*
 * lot3_npc/npc.c
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 3 — NPC / Enemy module — FULL IMPLEMENTATION
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "npc.h"
#include "../lot2_background/background.h"
#include "../lot1_joueur/joueur.h"

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */

void initNPC(NPC *e, SDL_Renderer *ren, int x, int y)
{
    if (!e) return;
    
    memset(e, 0, sizeof(NPC));
    e->rect.x = x;
    e->rect.y = y;
    e->rect.w = 50;
    e->rect.h = 50;
    e->x = (float)x;
    e->y = (float)y;
    e->vie = 3;
    e->active = 1;
    e->vitesse = 1.5f;
    strncpy(e->direction, "left", sizeof(e->direction) - 1);
    
    for (int i = 0; i < 4; i++) {
        // Fallback texture handle
        e->img[i] = IMG_LoadTexture(ren, "assets/images/npc.png");
    }
}

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */

void afficherNPC(SDL_Renderer *ren, NPC *ennemis, int nb)
{
    if (!ren || !ennemis || !backg) return;
    
    for (int i = 0; i < nb; i++) {
        NPC *e = &ennemis[i];
        if (!e->active) continue;
        
        SDL_Rect dest = {
            e->rect.x - backg->offsetX,
            e->rect.y - backg->offsetY,
            e->rect.w,
            e->rect.h
        };
        
        if (e->img[e->frame]) {
            SDL_RenderCopy(ren, e->img[e->frame], NULL, &dest);
        } else {
            // Fallback render (Red box)
            SDL_SetRenderDrawColor(ren, 255, 50, 50, 255);
            SDL_RenderFillRect(ren, &dest);
        }
        
        afficherSanteNPC(ren, e);
    }
}

void afficherSanteNPC(SDL_Renderer *ren, NPC *e)
{
    if (!ren || !e || !e->active || !backg) return;
    
    SDL_Rect healthBg = {
        e->rect.x - backg->offsetX,
        e->rect.y - backg->offsetY - 10,
        e->rect.w,
        5
    };
    
    SDL_Rect healthFg = healthBg;
    healthFg.w = (int)((e->vie / 3.0f) * e->rect.w);
    
    // Background (Black)
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, &healthBg);
    
    // Foreground (Green/Orange/Red)
    if (e->vie >= 3) {
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    } else if (e->vie == 2) {
        SDL_SetRenderDrawColor(ren, 255, 165, 0, 255);
    } else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    }
    
    SDL_RenderFillRect(ren, &healthFg);
}

/* ------------------------------------------------------------------ */
/*  Update / AI                                                         */
/* ------------------------------------------------------------------ */

void deplacerNPCAleatoire(NPC *e)
{
    if (!e || !e->active) return;
    
    // Simple horizontal patrol logic
    if (strcmp(e->direction, "left") == 0) {
        e->x -= e->vitesse;
        if (e->x < e->rect.x - 100) { // arbitrary patrol bound
            strncpy(e->direction, "right", sizeof(e->direction) - 1);
        }
    } else {
        e->x += e->vitesse;
        if (e->x > e->rect.x + 100) { // arbitrary patrol bound
            strncpy(e->direction, "left", sizeof(e->direction) - 1);
        }
    }
    
    e->rect.x = (int)e->x;
}

void animerNPC(NPC *e)
{
    if (!e || !e->active) return;
    
    static int tickCounter = 0;
    tickCounter++;
    
    if (tickCounter >= 10) {
        e->frame = (e->frame + 1) % 4;
        tickCounter = 0;
    }
}

void deplacerNPCAI(NPC *e, Joueur *j)
{
    if (!e || !j || !e->active) return;
    
    float dx = (float)(j->rect.x - e->rect.x);
    float dy = (float)(j->rect.y - e->rect.y);
    float dist = sqrtf(dx*dx + dy*dy);
    
    // Chase player if close, else patrol
    if (dist < 200.0f && abs((int)dy) < 50) { 
        if (dx > 0) {
            e->x += e->vitesse;
            strncpy(e->direction, "right", sizeof(e->direction) - 1);
        } else {
            e->x -= e->vitesse;
            strncpy(e->direction, "left", sizeof(e->direction) - 1);
        }
        e->rect.x = (int)e->x;
    } else {
        deplacerNPCAleatoire(e);
    }
}

void blesserNPC(NPC *e, int degats)
{
    if (!e || !e->active) return;
    
    e->vie -= degats;
    if (e->vie <= 0) {
        e->active = 0;
    }
}

/* ------------------------------------------------------------------ */
/*  Collision (AABB)                                                    */
/* ------------------------------------------------------------------ */

int detectionBB(SDL_Rect *a, SDL_Rect *b)
{
    if (!a || !b) return 0;
    
    if (a->x + a->w <= b->x ||
        b->x + b->w <= a->x ||
        a->y + a->h <= b->y ||
        b->y + b->h <= a->y) {
        return 0;
    }
    
    return 1;
}
