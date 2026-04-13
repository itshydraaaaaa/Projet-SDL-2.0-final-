#ifndef ENIGME1_H
#define ENIGME1_H

#include "../common.h"

void initEnigme1(const char *fichier);
void afficherEnigme1(SDL_Renderer *ren, TTF_Font *font, Question *q);
void afficherChronometre(SDL_Renderer *ren, Uint32 startTime, int duree);
Question *genererQuestion(Question *questions, int nb);
void resolveEnigme1(Joueur *j, int reponseDonnee, int bonneReponse);
int gererClicEnigme(Question *q, int x, int y);
void mettreAJourTempsEnigme(Question *q);
void libererTexturesEnigme(Question *q);
void initialiserTexturesEnigme(SDL_Renderer *renderer, Question *q, TTF_Font *font);
void afficherMessageReponse(SDL_Renderer *renderer, int succes, Uint32 duree_ms, TTF_Font *font);
int getNbQuestions(void);
Question* getQuestions(void);

#endif
