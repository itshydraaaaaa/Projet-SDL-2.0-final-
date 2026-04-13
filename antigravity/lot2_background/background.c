/*
 * lot2_background/background.c
 * ESPRIT — Algorithmic & Programming, 1A, 2020-2021
 *
 * LOT 2 — Background module — FULL IMPLEMENTATION
 *
 * Responsibilities:
 *   • Manage the global Background singleton (scrolling, timer, state)
 *   • Load / render one or two level background images
 *   • Render, update and free platforms
 *   • Manage the game timer and its visual display
 *   • Manage the key-bindings guide overlay
 *   • Manage the top-5 scores sub-menu
 *   • Persist scores to scores.dat (binary)
 *   • Handle text input for player names
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "background.h"

/* ------------------------------------------------------------------ */
/*  Internal constants                                                  */
/* ------------------------------------------------------------------ */
#define SCORES_FILE   "scores.dat"
#define MAX_SCORES    5

/* ------------------------------------------------------------------ */
/*  Global singleton                                                    */
/* ------------------------------------------------------------------ */
Background *backg = NULL;

/* ------------------------------------------------------------------ */
/*  Internal state (guide overlay)                                      */
/* ------------------------------------------------------------------ */
typedef struct {
    SDL_Texture *lines[10];
    SDL_Rect     rects[10];
    int          count;
    SDL_Rect     overlayRect;
} GuideOverlay;

static GuideOverlay guide = { {NULL}, {{0,0,0,0}}, 0, {0,0,0,0} };

/* Second level background texture (optional) */
static SDL_Texture *bgTex2 = NULL;

/* Scores sub-menu textures */
static SDL_Texture *scoreTextures[MAX_SCORES];
static SDL_Rect     scoreRects[MAX_SCORES];
static int          scoreCount = 0;

/* ------------------------------------------------------------------ */
/*  Helper: load a texture from file (returns NULL on failure)          */
/* ------------------------------------------------------------------ */
static SDL_Texture *loadTexture(SDL_Renderer *ren, const char *path)
{
    if (!path || !ren) return NULL;
    SDL_Texture *tex = IMG_LoadTexture(ren, path);
    if (!tex)
        fprintf(stderr, "[background] IMG_LoadTexture(\"%s\"): %s\n",
                path, IMG_GetError());
    return tex;
}

/* ------------------------------------------------------------------ */
/*  Helper: render a text string as a texture                           */
/*  Returns NULL on failure. Caller owns the texture.                   */
/* ------------------------------------------------------------------ */
static SDL_Texture *renderText(SDL_Renderer *ren, TTF_Font *font,
                                const char *text, SDL_Color color,
                                SDL_Rect *outRect)
{
    if (!font || !text) return NULL;
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return NULL;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    if (outRect) {
        outRect->w = surf->w;
        outRect->h = surf->h;
    }
    SDL_FreeSurface(surf);
    return tex;
}

/* ------------------------------------------------------------------ */
/*  Score sorting helper (insertion sort, descending by score)          */
/* ------------------------------------------------------------------ */
static void sortScores(Score *scores, int nb)
{
    for (int i = 1; i < nb; i++) {
        Score key = scores[i];
        int j = i - 1;
        while (j >= 0 && scores[j].score < key.score) {
            scores[j + 1] = scores[j];
            j--;
        }
        scores[j + 1] = key;
    }
}

/* ================================================================== */
/*  INITIALISATION                                                      */
/* ================================================================== */

void initBackground(SDL_Renderer *ren,
                    const char   *imgPath1,
                    const char   *imgPath2)
{
    /* Allocate singleton */
    backg = (Background *)malloc(sizeof(Background));
    if (!backg) {
        fprintf(stderr, "[background] malloc failed for Background\n");
        exit(EXIT_FAILURE);
    }
    memset(backg, 0, sizeof(Background));

    /* Load level 1 texture */
    backg->img = loadTexture(ren, imgPath1);

    /* Query texture dimensions for the map size */
    if (backg->img) {
        int w, h;
        SDL_QueryTexture(backg->img, NULL, NULL, &w, &h);
        backg->mapW = w;
        backg->mapH = h;
    } else {
        /* Fallback: use a larger-than-screen default map */
        backg->mapW = 2400;
        backg->mapH = 600;
    }

    /* Load optional level 2 texture */
    bgTex2 = loadTexture(ren, imgPath2);

    /* Initial state */
    backg->offsetX   = 0;
    backg->offsetY   = 0;
    backg->volume    = 80;
    backg->mode      = 2; /* windowed */
    backg->elapsedSec = 0;
    backg->limiteSec  = 0; /* 0 = count-up */
    strncpy(backg->niveau, "Menu", sizeof(backg->niveau) - 1);
    backg->niveau[sizeof(backg->niveau) - 1] = '\0';
}

/* ------------------------------------------------------------------ */

void initPlateforme(Plateforme    *p,
                    SDL_Renderer  *ren,
                    const char    *imgPath,
                    int x, int y, int w, int h,
                    TypePlateforme type,
                    int dx, int dy,
                    int minX, int maxX)
{
    if (!p) return;
    memset(p, 0, sizeof(Plateforme));

    p->img    = loadTexture(ren, imgPath);
    p->rect.x = x;
    p->rect.y = y;
    p->rect.w = w;
    p->rect.h = h;
    p->type   = type;
    p->active = 1;
    p->dx     = dx;
    p->dy     = dy;
    p->minX   = minX;
    p->maxX   = maxX;
}

/* ------------------------------------------------------------------ */

void initTimer(void)
{
    if (!backg) return;
    backg->startTime  = SDL_GetTicks();
    backg->elapsedSec = 0;
}

/* ------------------------------------------------------------------ */

void initGuide(SDL_Renderer *ren, TTF_Font *font)
{
    if (!ren) return;

    /* Key bindings to display */
    static const char *bindings[] = {
        "  H       — Toggle this guide",
        "  Arrow   — Move player 1",
        "  W/A/D   — Move player 2",
        "  ESC     — Quit",
        "  F       — Toggle fullscreen",
        NULL
    };

    SDL_Color white = {255, 255, 255, 255};
    int lineH = 28;
    int startY = 160;
    int startX = 220;

    guide.count = 0;
    guide.overlayRect = (SDL_Rect){200, 140, 400, 200};

    for (int i = 0; bindings[i] != NULL && guide.count < 10; i++) {
        SDL_Rect r = {startX, startY + i * lineH, 0, 0};
        guide.lines[guide.count] = renderText(ren, font, bindings[i], white, &r);
        guide.rects[guide.count] = r;
        guide.count++;
    }
}

/* ------------------------------------------------------------------ */

void initSousMenuScores(SDL_Renderer *ren, TTF_Font *font)
{
    if (!ren) return;

    Score scores[MAX_SCORES];
    int nb = 0;
    chargerScores(scores, &nb);

    SDL_Color gold  = {255, 215, 0, 255};
    int startY = 220;

    scoreCount = 0;
    for (int i = 0; i < nb && i < MAX_SCORES; i++) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%d.  %-20s  score: %d   time: %ds",
                 i + 1, scores[i].nom, scores[i].score, scores[i].temps);
        SDL_Rect r = {150, startY + i * 32, 0, 0};
        scoreTextures[i] = renderText(ren, font, buf, gold, &r);
        scoreRects[i]    = r;
        scoreCount++;
    }
}

/* ================================================================== */
/*  AFFICHAGE                                                           */
/* ================================================================== */

void afficherBackground(SDL_Renderer *ren, int mode)
{
    if (!ren || !backg) return;

    /* Choose texture based on current niveau */
    SDL_Texture *tex = backg->img;
    if (strcmp(backg->niveau, "Jouer") == 0 && bgTex2 != NULL)
        tex = bgTex2;

    if (!tex) {
        /* No texture: fill with a dark blue fallback */
        SDL_SetRenderDrawColor(ren, 10, 20, 50, 255);
        SDL_RenderClear(ren);
        return;
    }

    /* Query actual window size so the function works at any resolution */
    int screenW = 800, screenH = 600;
    SDL_GetRendererOutputSize(ren, &screenW, &screenH);

    if (mode == 1) {
        /* ── Single-screen ──────────────────────────────────────── */
        SDL_Rect src = {
            backg->offsetX,
            backg->offsetY,
            screenW,
            screenH
        };
        /* Clamp source rect to texture bounds */
        int tw, th;
        SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
        if (src.x + src.w > tw) src.w = tw - src.x;
        if (src.y + src.h > th) src.h = th - src.y;

        SDL_Rect dst = {0, 0, src.w, src.h};
        SDL_RenderCopy(ren, tex, &src, &dst);

    } else {
        /* ── Split-screen (mode == 2) ────────────────────────────── */
        int halfW = screenW / 2;

        int tw, th;
        SDL_QueryTexture(tex, NULL, NULL, &tw, &th);

        /* --- Left half: Player 1 viewport --- */
        SDL_Rect leftVP  = {0, 0, halfW, screenH};
        SDL_RenderSetViewport(ren, &leftVP);

        SDL_Rect srcLeft = {
            backg->offsetX,
            backg->offsetY,
            halfW,
            screenH
        };
        if (srcLeft.x + srcLeft.w > tw) srcLeft.w = tw - srcLeft.x;
        if (srcLeft.y + srcLeft.h > th) srcLeft.h = th - srcLeft.y;

        SDL_Rect dstLeft = {0, 0, srcLeft.w, srcLeft.h};
        SDL_RenderCopy(ren, tex, &srcLeft, &dstLeft);

        /* --- Right half: Player 2 viewport (independent offset) --- */
        SDL_Rect rightVP = {halfW, 0, halfW, screenH};
        SDL_RenderSetViewport(ren, &rightVP);

        SDL_Rect srcRight = srcLeft; /* Lot 1 should update for P2 camera */
        SDL_Rect dstRight = {0, 0, srcRight.w, srcRight.h};
        SDL_RenderCopy(ren, tex, &srcRight, &dstRight);

        /* --- Separator line at center --- */
        SDL_RenderSetViewport(ren, NULL); /* reset to full window */
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(ren, halfW,     0, halfW,     screenH);
        SDL_RenderDrawLine(ren, halfW + 1, 0, halfW + 1, screenH);
    }

    /* Reset viewport to full window */
    SDL_RenderSetViewport(ren, NULL);
}

/* ------------------------------------------------------------------ */

void afficherPlateformes(SDL_Renderer *ren, Plateforme *plateformes, int nb)
{
    if (!ren || !plateformes || !backg) return;

    for (int i = 0; i < nb; i++) {
        Plateforme *p = &plateformes[i];
        if (!p->active) continue;

        SDL_Rect dest = {
            p->rect.x - backg->offsetX,
            p->rect.y - backg->offsetY,
            p->rect.w,
            p->rect.h
        };

        if (p->img) {
            SDL_RenderCopy(ren, p->img, NULL, &dest);
        } else {
            /* Fallback: colored rectangle by type */
            switch (p->type) {
                case FIXE:         SDL_SetRenderDrawColor(ren, 139, 90, 43, 255); break;
                case MOBILE:       SDL_SetRenderDrawColor(ren,  50, 150, 255, 255); break;
                case DESTRUCTIBLE: SDL_SetRenderDrawColor(ren, 200, 80,  80, 255); break;
            }
            SDL_RenderFillRect(ren, &dest);
        }
    }
}

/* ------------------------------------------------------------------ */

void afficherTemps(SDL_Renderer *ren, TTF_Font *font)
{
    if (!ren || !font || !backg) return;

    int secs = backg->elapsedSec;
    int mm   = secs / 60;
    int ss   = secs % 60;

    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);

    /* Flash red if within 10 seconds of the limit */
    SDL_Color color = {220, 220, 220, 255}; /* default: light-grey */
    if (backg->limiteSec > 0) {
        int remaining = backg->limiteSec - secs;
        if (remaining <= 10) {
            /* Flash: alternate red and white every ~30 frames (500ms) */
            Uint32 ticks = SDL_GetTicks();
            color = (ticks / 500) % 2 == 0
                ? (SDL_Color){255, 40, 40, 255}
                : (SDL_Color){255, 255, 255, 255};
        }
    }

    int screenW = 800;
    SDL_GetRendererOutputSize(ren, &screenW, NULL);
    SDL_Rect rect = {screenW / 2 - 40, 10, 0, 0};
    SDL_Texture *tex = renderText(ren, font, buf, color, &rect);
    if (tex) {
        SDL_RenderCopy(ren, tex, NULL, &rect);
        SDL_DestroyTexture(tex);
    }
}

/* ------------------------------------------------------------------ */

void afficherGuide(SDL_Renderer *ren)
{
    if (!ren) return;

    /* Semi-transparent dark overlay */
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
    SDL_RenderFillRect(ren, &guide.overlayRect);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

    /* Title */
    /* (font not stored here; lines were pre-rendered in initGuide) */
    for (int i = 0; i < guide.count; i++) {
        if (guide.lines[i])
            SDL_RenderCopy(ren, guide.lines[i], NULL, &guide.rects[i]);
    }
}

/* ------------------------------------------------------------------ */

void afficherSousMenuScores(SDL_Renderer *ren, TTF_Font *font)
{
    if (!ren) return;
    (void)font; /* already pre-rendered in initSousMenuScores */

    SDL_Rect panel;
    int sw = 800, sh = 600;
    SDL_GetRendererOutputSize(ren, &sw, &sh);
    panel.x = 100; panel.y = 150;
    panel.w = sw - 200; panel.h = sh - 200;
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 20, 20, 40, 210);
    SDL_RenderFillRect(ren, &panel);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

    /* Border */
    SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
    SDL_RenderDrawRect(ren, &panel);

    /* Score rows */
    for (int i = 0; i < scoreCount; i++) {
        if (scoreTextures[i])
            SDL_RenderCopy(ren, scoreTextures[i], NULL, &scoreRects[i]);
    }
}

/* ================================================================== */
/*  UPDATE                                                              */
/* ================================================================== */

void updateScrolling(int playerX, int playerY, int screenW, int screenH)
{
    if (!backg) return;

    int halfW = screenW / 2;
    int halfH = screenH / 2;

    /* ── Horizontal scrolling ─────────────────────────────────── */
    if (playerX < halfW) {
        /* Player in left zone: no horizontal scroll */
        backg->offsetX = 0;
    } else if (playerX < backg->mapW - halfW) {
        /* Player in middle zone: follow player */
        backg->offsetX = playerX - halfW;
    } else {
        /* Player near right edge: clamp */
        backg->offsetX = backg->mapW - screenW;
    }

    /* Hard clamp */
    backg->offsetX = SDL_max(0,
                    SDL_min(backg->offsetX, backg->mapW - screenW));

    /* ── Vertical scrolling ───────────────────────────────────── */
    if (playerY < halfH) {
        backg->offsetY = 0;
    } else if (playerY < backg->mapH - halfH) {
        backg->offsetY = playerY - halfH;
    } else {
        backg->offsetY = backg->mapH - screenH;
    }

    backg->offsetY = SDL_max(0,
                    SDL_min(backg->offsetY, backg->mapH - screenH));
}

/* ------------------------------------------------------------------ */

void updatePlateformesMobiles(Plateforme *plateformes, int nb)
{
    if (!plateformes) return;

    for (int i = 0; i < nb; i++) {
        Plateforme *p = &plateformes[i];
        if (!p->active || p->type != MOBILE) continue;

        p->rect.x += p->dx;
        p->rect.y += p->dy;

        /* Bounce at horizontal boundaries */
        if (p->dx != 0) {
            if (p->rect.x <= p->minX) {
                p->rect.x = p->minX;
                p->dx = -p->dx;
            } else if (p->rect.x + p->rect.w >= p->maxX) {
                p->rect.x = p->maxX - p->rect.w;
                p->dx = -p->dx;
            }
        }
    }
}

/* ------------------------------------------------------------------ */

void updateTimer(void)
{
    if (!backg) return;
    backg->elapsedSec =
        (int)((SDL_GetTicks() - backg->startTime) / 1000u);
}

/* ------------------------------------------------------------------ */

void detruirePlateforme(Plateforme *p)
{
    if (!p || p->type != DESTRUCTIBLE) return;
    p->active = 0;
    /* Texture is kept so Lot 4 can play a destruction animation;
       it will be freed by libererPlateformes at program exit. */
}

/* ================================================================== */
/*  SCORES                                                              */
/* ================================================================== */

void chargerScores(Score *scores, int *nb)
{
    if (!scores || !nb) return;
    *nb = 0;

    FILE *f = fopen(SCORES_FILE, "rb");
    if (!f) return; /* no file yet → zero records is valid */

    *nb = (int)fread(scores, sizeof(Score), MAX_SCORES, f);
    fclose(f);

    /* Ensure they are sorted (file might be written by an older version) */
    sortScores(scores, *nb);
}

/* ------------------------------------------------------------------ */

void sauvegarderScore(const char *nom, int score, int temps)
{
    if (!nom) return;

    /* Load existing top scores */
    Score scores[MAX_SCORES + 1]; /* +1 for the new entry */
    int nb = 0;
    chargerScores(scores, &nb);

    /* Append the new entry */
    if (nb < MAX_SCORES + 1) {
        strncpy(scores[nb].nom, nom, sizeof(scores[nb].nom) - 1);
        scores[nb].nom[sizeof(scores[nb].nom) - 1] = '\0';
        scores[nb].score = score;
        scores[nb].temps = temps;
        nb++;
    }

    /* Sort descending by score and keep top 5 */
    sortScores(scores, nb);
    if (nb > MAX_SCORES) nb = MAX_SCORES;

    /* Write back */
    FILE *f = fopen(SCORES_FILE, "wb");
    if (!f) {
        fprintf(stderr, "[background] Cannot open %s for writing\n", SCORES_FILE);
        return;
    }
    fwrite(scores, sizeof(Score), nb, f);
    fclose(f);
}

/* ------------------------------------------------------------------ */

void saisirNomJoueur(SDL_Renderer *ren, TTF_Font *font,
                     char *nom, int maxLen)
{
    if (!ren || !font || !nom || maxLen <= 0) return;

    int len = 0;
    nom[0]  = '\0';

    SDL_Color white  = {255, 255, 255, 255};
    SDL_Color yellow = {255, 230, 80, 255};

    SDL_StartTextInput();

    int done = 0;
    SDL_Event ev;

    while (!done) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) { done = 1; break; }

            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_RETURN ||
                    ev.key.keysym.sym == SDLK_RETURN2 ||
                    ev.key.keysym.sym == SDLK_KP_ENTER) {
                    done = 1;
                } else if (ev.key.keysym.sym == SDLK_BACKSPACE && len > 0) {
                    nom[--len] = '\0';
                }
            }
            if (ev.type == SDL_TEXTINPUT) {
                int addLen = (int)strlen(ev.text.text);
                if (len + addLen < maxLen - 1) {
                    strcat(nom, ev.text.text);
                    len += addLen;
                }
            }
        }

        /* Render input screen */
        SDL_SetRenderDrawColor(ren, 15, 15, 30, 255);
        SDL_RenderClear(ren);

        SDL_Rect r1 = {250, 200, 0, 0};
        SDL_Texture *prompt = renderText(ren, font, "Enter your name:", white, &r1);
        if (prompt) { SDL_RenderCopy(ren, prompt, NULL, &r1); SDL_DestroyTexture(prompt); }

        /* Input text with blinking cursor */
        char display[maxLen + 2];
        Uint32 ticks = SDL_GetTicks();
        snprintf(display, sizeof(display), "%s%s",
                 nom, (ticks / 500) % 2 == 0 ? "_" : " ");

        SDL_Rect r2 = {250, 240, 0, 0};
        SDL_Texture *input = renderText(ren, font, display, yellow, &r2);
        if (input) { SDL_RenderCopy(ren, input, NULL, &r2); SDL_DestroyTexture(input); }

        SDL_Rect r3 = {220, 290, 0, 0};
        SDL_Texture *hint = renderText(ren, font, "Press ENTER to confirm", white, &r3);
        if (hint) { SDL_RenderCopy(ren, hint, NULL, &r3); SDL_DestroyTexture(hint); }

        SDL_RenderPresent(ren);
        SDL_Delay(16); /* ~60 fps */
    }

    SDL_StopTextInput();
}

/* ================================================================== */
/*  LIBÉRATION                                                          */
/* ================================================================== */

void libererBackground(void)
{
    if (!backg) return;

    if (backg->img) {
        SDL_DestroyTexture(backg->img);
        backg->img = NULL;
    }
    if (bgTex2) {
        SDL_DestroyTexture(bgTex2);
        bgTex2 = NULL;
    }

    /* Free score sub-menu textures */
    for (int i = 0; i < scoreCount; i++) {
        if (scoreTextures[i]) {
            SDL_DestroyTexture(scoreTextures[i]);
            scoreTextures[i] = NULL;
        }
    }
    scoreCount = 0;

    free(backg);
    backg = NULL;
}

/* ------------------------------------------------------------------ */

void libererPlateformes(Plateforme *plateformes, int nb)
{
    if (!plateformes) return;
    for (int i = 0; i < nb; i++) {
        if (plateformes[i].img) {
            SDL_DestroyTexture(plateformes[i].img);
            plateformes[i].img = NULL;
        }
    }
}

/* ------------------------------------------------------------------ */

void libererGuide(void)
{
    for (int i = 0; i < guide.count; i++) {
        if (guide.lines[i]) {
            SDL_DestroyTexture(guide.lines[i]);
            guide.lines[i] = NULL;
        }
    }
    guide.count = 0;
}
