/*
 * lot6_enigme2/enigme2.h
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 6 — Visual puzzle (image riddle) module.
 */

#ifndef ENIGME2_H
#define ENIGME2_H

#include "../common.h"
#include "../lot1_joueur/joueur.h"

/* ------------------------------------------------------------------ */
/*  Puzzle set struct (local to Lot 6)                                  */
/* ------------------------------------------------------------------ */
typedef struct {
    SDL_Texture *imgMissing;
    SDL_Texture *propositions[3];
    SDL_Texture *imgFull;
    int          bonneReponse; /* 0, 1, or 2 */
    int          dejaVue;
} PuzzleSet;

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */
void initEnigme2(SDL_Renderer *ren);
void initChronometreP(int duree);

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */
void afficherEnigme2(SDL_Renderer *ren, PuzzleSet *p);
void afficherChronometreP(SDL_Renderer *ren);
void afficherRoutozoom(SDL_Renderer *ren, SDL_Texture *msg, int success);

/* ------------------------------------------------------------------ */
/*  Logic                                                               */
/* ------------------------------------------------------------------ */
PuzzleSet *genererPuzzle(PuzzleSet *puzzles, int nb);
void       resolveEnigme2(Joueur *j, SDL_Event *e,
                           PuzzleSet *p, SDL_Rect *dropZone);

#endif /* ENIGME2_H */
