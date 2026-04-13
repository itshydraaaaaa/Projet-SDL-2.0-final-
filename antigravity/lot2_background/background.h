/*
 * lot2_background/background.h
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 *
 * LOT 2 — Background module public interface.
 * Implement all bodies in background.c.
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../common.h"

/* ------------------------------------------------------------------ */
/*  Global singleton — defined once in background.c                    */
/* ------------------------------------------------------------------ */
extern Background *backg;

/* ------------------------------------------------------------------ */
/*  Initialisation                                                      */
/* ------------------------------------------------------------------ */

/**
 * Allocate and initialise the global Background.
 * Loads two level images; imgPath2 may be NULL if only one level exists.
 */
void initBackground(SDL_Renderer *ren,
                    const char   *imgPath1,
                    const char   *imgPath2);

/**
 * Initialise a single platform.
 * type  : FIXE | MOBILE | DESTRUCTIBLE
 * dx,dy : movement velocity (MOBILE only, use 0 for others)
 * minX,maxX : horizontal movement bounds (MOBILE only)
 */
void initPlateforme(Plateforme    *p,
                    SDL_Renderer  *ren,
                    const char    *imgPath,
                    int x, int y, int w, int h,
                    TypePlateforme type,
                    int dx, int dy,
                    int minX, int maxX);

/** Record the start time for the game timer. */
void initTimer(void);

/** Initialise the in-game help / key-bindings overlay. */
void initGuide(SDL_Renderer *ren, TTF_Font *font);

/** Initialise the top-scores sub-menu overlay. */
void initSousMenuScores(SDL_Renderer *ren, TTF_Font *font);

/* ------------------------------------------------------------------ */
/*  Affichage (rendering)                                               */
/* ------------------------------------------------------------------ */

/**
 * Render the background.
 * mode 1 → full-screen single player
 * mode 2 → split-screen (left = P1, right = P2)
 */
void afficherBackground(SDL_Renderer *ren, int mode);

/** Render all active platforms. */
void afficherPlateformes(SDL_Renderer *ren, Plateforme *plateformes, int nb);

/** Render the elapsed / countdown timer in MM:SS format. */
void afficherTemps(SDL_Renderer *ren, TTF_Font *font);

/** Render the key-bindings guide overlay (toggled by SDLK_h). */
void afficherGuide(SDL_Renderer *ren);

/** Render the high-scores sub-menu overlay. */
void afficherSousMenuScores(SDL_Renderer *ren, TTF_Font *font);

/* ------------------------------------------------------------------ */
/*  Update                                                              */
/* ------------------------------------------------------------------ */

/**
 * Adjust backg->offsetX/Y so the camera follows the player.
 * screenW / screenH are the window dimensions.
 */
void updateScrolling(int playerX, int playerY, int screenW, int screenH);

/** Move all MOBILE platforms and bounce them at their boundaries. */
void updatePlateformesMobiles(Plateforme *plateformes, int nb);

/** Update backg->elapsedSec from SDL_GetTicks. */
void updateTimer(void);

/** Mark a DESTRUCTIBLE platform as destroyed (active = 0). */
void detruirePlateforme(Plateforme *p);

/* ------------------------------------------------------------------ */
/*  Scores                                                              */
/* ------------------------------------------------------------------ */

/** Append a new score to scores.dat (binary), keep top 5 sorted desc. */
void sauvegarderScore(const char *nom, int score, int temps);

/**
 * Load up to 5 Score records from scores.dat into the provided array.
 * *nb receives the actual number of records loaded.
 */
void chargerScores(Score *scores, int *nb);

/**
 * Interactive SDL event loop for entering a player name.
 * Reads printable SDLK characters into nom[] (max maxLen-1 chars),
 * rendering the current input live on screen.
 */
void saisirNomJoueur(SDL_Renderer *ren, TTF_Font *font,
                     char *nom, int maxLen);

/* ------------------------------------------------------------------ */
/*  Libération (cleanup)                                                */
/* ------------------------------------------------------------------ */

/** Destroy the background texture and free the Background struct. */
void libererBackground(void);

/** Destroy all platform textures. */
void libererPlateformes(Plateforme *plateformes, int nb);

/** Free guide overlay resources. */
void libererGuide(void);

#endif /* BACKGROUND_H */
