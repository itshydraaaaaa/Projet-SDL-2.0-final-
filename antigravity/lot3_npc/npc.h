/*
 * lot3_npc/npc.h
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 3 — NPC / Enemy module public interface.
 */

#ifndef NPC_H
#define NPC_H

#include "../common.h"

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */
void initNPC(NPC *e, SDL_Renderer *ren, int x, int y);

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */
void afficherNPC(SDL_Renderer *ren, NPC *ennemis, int nb);
void afficherSanteNPC(SDL_Renderer *ren, NPC *e);

/* ------------------------------------------------------------------ */
/*  Update / AI                                                         */
/* ------------------------------------------------------------------ */
void deplacerNPCAleatoire(NPC *e);
void animerNPC(NPC *e);
void deplacerNPCAI(NPC *e, Joueur *j);
void blesserNPC(NPC *e, int degats);

/* ------------------------------------------------------------------ */
/*  Collision (generic bounding-box, used by Lot 1 & Lot 4)            */
/* ------------------------------------------------------------------ */
int detectionBB(SDL_Rect *a, SDL_Rect *b);

#endif /* NPC_H */
