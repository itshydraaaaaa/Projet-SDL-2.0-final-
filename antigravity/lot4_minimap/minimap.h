/*
 * lot4_minimap/minimap.h
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 4 — Minimap, advanced collisions, save/load.
 */

#ifndef MINIMAP_H
#define MINIMAP_H

#include "../common.h"
#include "../lot1_joueur/joueur.h"
#include "../lot3_npc/npc.h"
#include "../lot2_background/background.h"

/* ------------------------------------------------------------------ */
/*  Initialisation                                                    */
/* ------------------------------------------------------------------ */
void initMiniMap(SDL_Renderer *ren);
void initSousMenuSauvegarde(SDL_Renderer *ren);

/* ------------------------------------------------------------------ */
/*  Affichage                                                         */
/* ------------------------------------------------------------------ */
void afficherMiniMap(SDL_Renderer *ren, Joueur *j,
                     NPC *ennemis, int nbEnnemis);
void afficherSousMenuSauvegarde(SDL_Renderer *ren);

/* ------------------------------------------------------------------ */
/*  Update                                                            */
/* ------------------------------------------------------------------ */
void majMiniMap(Joueur *j1, Joueur *j2);

/* ------------------------------------------------------------------ */
/*  Collisions                                                        */
/* ------------------------------------------------------------------ */
int collisionPixelParfait(SDL_Texture *bg, SDL_Rect *rect,
                          int offsetX, int offsetY);
int collisionPlatformeMobile(SDL_Rect *joueur, Plateforme *p);
void animerCollisionBackground(SDL_Renderer *ren, int x, int y);

/* ------------------------------------------------------------------ */
/*  Nettoyage                                                         */
/* ------------------------------------------------------------------ */
void freeMiniMap(void);

#endif /* MINIMAP_H */
