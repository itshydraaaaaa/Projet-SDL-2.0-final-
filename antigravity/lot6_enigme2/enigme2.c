/*
 * lot6_enigme2/enigme2.c
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 6 — Visual puzzle module — FULL IMPLEMENTATION
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigme2.h"
#include "../lot2_background/background.h"

// Internal global state for Lot 6
static PuzzleSet ps[5];
static Uint32 chronoStart = 0;
static int    chronoLimit = 0;

static SDL_Rect draggedRect;
static int isDragging = 0;
static int draggedIndex = -1;
static SDL_Texture *draggedTex = NULL;

/* ------------------------------------------------------------------ */
/*  Helper: Fisher-Yates array shuffle for propositions                 */
/* ------------------------------------------------------------------ */
static void shufflePropositions(PuzzleSet *p) {
    if (!p) return;
    for (int i = 2; i > 0; i--) {
        int j = rand() % (i + 1);
        SDL_Texture *t = p->propositions[i];
        p->propositions[i] = p->propositions[j];
        p->propositions[j] = t;
        
        // Follow the index of the correct answer
        if (p->bonneReponse == i) {
            p->bonneReponse = j;
        } else if (p->bonneReponse == j) {
            p->bonneReponse = i;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */

void initEnigme2(SDL_Renderer *ren)
{
    if (!ren) return;
    memset(ps, 0, sizeof(ps));
    
    for (int i = 0; i < 5; i++) {
        // Fallback or actual image logic
        char path[128];
        snprintf(path, sizeof(path), "assets/images/puzzles/puzzle_%d", i);
        
        ps[i].imgMissing = IMG_LoadTexture(ren, "assets/images/puzzles/p_missing.png");
        ps[i].imgFull    = IMG_LoadTexture(ren, "assets/images/puzzles/p_full.png");
        ps[i].propositions[0] = IMG_LoadTexture(ren, "assets/images/puzzles/prop_A.png");
        ps[i].propositions[1] = IMG_LoadTexture(ren, "assets/images/puzzles/prop_B.png");
        ps[i].propositions[2] = IMG_LoadTexture(ren, "assets/images/puzzles/prop_C.png");
        
        ps[i].bonneReponse = 0; // Assume A is correct initially
        ps[i].dejaVue = 0;
    }
}

void initChronometreP(int duree)
{
    chronoStart = SDL_GetTicks();
    chronoLimit = duree;
}

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */

void afficherEnigme2(SDL_Renderer *ren, PuzzleSet *p)
{
    if (!ren || !p) return;
    
    // Background dim
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);
    int screenW = 800, screenH = 600;
    SDL_GetRendererOutputSize(ren, &screenW, &screenH);
    SDL_Rect fullR = {0,0,screenW,screenH};
    SDL_RenderFillRect(ren, &fullR);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    
    // Draw missing puzzle
    SDL_Rect rMissing = { 100, 150, 300, 300 };
    if (p->imgMissing) {
        SDL_RenderCopy(ren, p->imgMissing, NULL, &rMissing);
    } else {
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderFillRect(ren, &rMissing);
    }
    
    // Draw dropzone indicator
    SDL_Rect dropZone = { 200, 250, 100, 100 }; // Example arbitrary area
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 128);
    SDL_RenderDrawRect(ren, &dropZone);
    
    // Draw propositions
    for (int i=0; i<3; i++) {
        if (isDragging && draggedIndex == i) continue; // Skip rendering here if dragged
        
        SDL_Rect rProp = { 500, 100 + i*160, 150, 150 };
        if (p->propositions[i]) {
            SDL_RenderCopy(ren, p->propositions[i], NULL, &rProp);
        } else {
            SDL_SetRenderDrawColor(ren, 200, 200, 50, 255);
            SDL_RenderFillRect(ren, &rProp);
        }
    }
    
    // Draw dragged piece (follows mouse)
    if (isDragging && draggedTex) {
        SDL_RenderCopy(ren, draggedTex, NULL, &draggedRect);
    }
}

void afficherChronometreP(SDL_Renderer *ren)
{
    if (!ren || chronoLimit <= 0) return;
    
    Uint32 currentTicks = SDL_GetTicks();
    int elapsed = (currentTicks - chronoStart) / 1000;
    int remaining = chronoLimit - elapsed;
    if (remaining < 0) remaining = 0;
    
    float ratio = (float)remaining / (float)chronoLimit;
    SDL_Rect barBg = { 100, 50, 600, 20 };
    SDL_Rect barFg = barBg;
    barFg.w = (int)(600 * ratio);
    
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    SDL_RenderFillRect(ren, &barBg);
    
    // Pulse effect
    Uint8 pulse = (currentTicks % 500) > 250 ? 255 : 150;
    
    if (ratio > 0.5f) {
        SDL_SetRenderDrawColor(ren, 0, pulse, 0, 255);
    } else if (ratio > 0.2f) {
        SDL_SetRenderDrawColor(ren, pulse, pulse, 0, 255);
    } else {
        SDL_SetRenderDrawColor(ren, pulse, 0, 0, 255);
    }
    
    SDL_RenderFillRect(ren, &barFg);
}

void afficherRoutozoom(SDL_Renderer *ren, SDL_Texture *msg, int success)
{
    if (!ren) return;
    (void)success; // Maybe change color logic
    
    int screenW = 800, screenH = 600;
    SDL_GetRendererOutputSize(ren, &screenW, &screenH);
    
    for (int frame = 0; frame < 60; frame++) {
        double angle = frame * 6.0; // 360 / 60
        float scale = (float)frame / 60.0f;
        if (scale < 0.1f) scale = 0.1f;
        
        SDL_Rect tgt;
        tgt.w = (int)(300 * scale);
        tgt.h = (int)(150 * scale);
        tgt.x = (screenW/2) - (tgt.w/2);
        tgt.y = (screenH/2) - (tgt.h/2);
        
        SDL_RenderClear(ren);    
        
        if (msg) {
            SDL_RenderCopyEx(ren, msg, NULL, &tgt, angle, NULL, SDL_FLIP_NONE);
        } else {
            SDL_SetRenderDrawColor(ren, success ? 0 : 255, success ? 255 : 0, 0, 255);
            SDL_RenderFillRect(ren, &tgt); // Fallback color
        }
        
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

/* ------------------------------------------------------------------ */
/*  Logic                                                               */
/* ------------------------------------------------------------------ */

PuzzleSet *genererPuzzle(PuzzleSet *puzzles, int nb)
{
    if (!puzzles || nb <= 0) return NULL;
    
    // Find an unseen puzzle
    int idx = -1;
    for (int i = 0; i < nb; i++) {
        if (!puzzles[i].dejaVue) {
            idx = i;
            break;
        }
    }
    
    // If all seen, reset
    if (idx == -1) {
        for (int i = 0; i < nb; i++) puzzles[i].dejaVue = 0;
        idx = 0;
    }
    
    puzzles[idx].dejaVue = 1;
    shufflePropositions(&puzzles[idx]);
    
    return &puzzles[idx];
}

void resolveEnigme2(Joueur *j, SDL_Event *e, PuzzleSet *p, SDL_Rect *dropZone)
{
    if (!j || !e || !p || !dropZone) return;
    
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        // Check if clicking proposition
        for (int i=0; i<3; i++) {
            SDL_Rect rProp = { 500, 100 + i*160, 150, 150 };
            if (mx >= rProp.x && mx <= rProp.x + rProp.w && my >= rProp.y && my <= rProp.y + rProp.h) {
                isDragging = 1;
                draggedIndex = i;
                draggedTex = p->propositions[i];
                // Center grabbed image around cursor
                draggedRect.w = 150;
                draggedRect.h = 150;
                break;
            }
        }
    }
    else if (e->type == SDL_MOUSEMOTION && isDragging) {
        draggedRect.x = mx - 75;
        draggedRect.y = my - 75;
    }
    else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        if (isDragging) {
            // Check DropZone
            int center_x = draggedRect.x + 75;
            int center_y = draggedRect.y + 75;
            
            if (center_x >= dropZone->x && center_x <= dropZone->x + dropZone->w &&
                center_y >= dropZone->y && center_y <= dropZone->y + dropZone->h) {
                
                // Validate answer
                if (draggedIndex == p->bonneReponse) {
                    ajouterScore(j, 75);
                    afficherRoutozoom(NULL, NULL, 1);
                } else {
                    perdreVie(j);
                    afficherRoutozoom(NULL, NULL, 0);
                }
                
                if (backg) strncpy(backg->niveau, "Jouer", sizeof(backg->niveau) - 1);
            }
            
            isDragging = 0;
            draggedIndex = -1;
            draggedTex = NULL;
        }
    }
}
