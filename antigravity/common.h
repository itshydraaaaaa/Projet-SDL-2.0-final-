#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_QUESTIONS 100

typedef enum { FIXE, MOBILE, DESTRUCTIBLE } TypePlateforme;

typedef struct {
    SDL_Texture *img;
    SDL_Rect rect;
    TypePlateforme type;
    int active;
    int dx, dy;
    int minX, maxX;
} Plateforme;

typedef struct {
    SDL_Texture *img;
    int offsetX, offsetY;
    int mapW, mapH;
    char niveau[16];
    int volume;
    int mode;
    Uint32 startTime;
    int elapsedSec;
    int limiteSec;
} Background;

typedef struct {
    SDL_Texture *img[8];
    SDL_Rect rect;
    int frame;
    char direction[8];
    int vie;
    int score;
    float vitesse;
    int jumping;
    int grounded;
    float velY;
    float x, y;
} Joueur;

typedef struct {
    SDL_Texture *img[4];
    SDL_Rect rect;
    int frame;
    char direction[8];
    int vie;
    int active;
    float vitesse;
    float x, y;
} NPC;

typedef struct {
    char question[256];
    char choix[3][128];
    int bonneReponse;
    int dejaVue;
    SDL_Texture* texture_question;
    SDL_Texture* texture_reponse1;
    SDL_Texture* texture_reponse2;
    SDL_Texture* texture_reponse3;
    SDL_Rect rect_question;
    SDL_Rect rect_reponse1;
    SDL_Rect rect_reponse2;
    SDL_Rect rect_reponse3;
    Uint32 temps_debut;
    int temps_limite;
    int temps_ecoule;
    int etat;
} Question;
#endif
