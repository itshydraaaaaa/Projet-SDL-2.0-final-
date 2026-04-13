/*
 * lot1_joueur/joueur.h
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 * LOT 1 — Player module public interface.
 */

#ifndef JOUEUR_H
#define JOUEUR_H

#include "../common.h"

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */
void initJoueur(Joueur *j, SDL_Renderer *ren);
void initJoueur2(Joueur *j2, SDL_Renderer *ren);
void initSousMenuJoueur(SDL_Renderer *ren);

/* ------------------------------------------------------------------ */
/*  Affichage                                                           */
/* ------------------------------------------------------------------ */
void afficherJoueur(SDL_Renderer *ren, Joueur *j);
void afficherSousMenuJoueur(SDL_Renderer *ren);

/* ------------------------------------------------------------------ */
/*  Update / gameplay                                                   */
/* ------------------------------------------------------------------ */
void deplacerJoueur(Joueur *j, const char *input);
void animerJoueur(Joueur *j);
void perdreVie(Joueur *j);
void ajouterScore(Joueur *j, int points);
void updateJoueurPhysics(Joueur *j, Plateforme *plateformes, int nbPlateformes);

#endif /* JOUEUR_H */
