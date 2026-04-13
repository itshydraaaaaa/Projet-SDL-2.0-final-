# Lot 6 — Enigme 2 : Puzzle / Image (Visual Riddle Module)

**Branch:** `feature/lot6-enigme2`
**Total points:** 8 pts
**Files you own:** `enigme2.h`, `enigme2.c`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

Your module handles image-based riddles: the player sees an image with a missing piece (or
a shadow/silhouette) and must identify or drag the correct piece to the right position.

### Task 6.1 — Load puzzle images (1 pt)
Prepare **5 puzzle sets** in `assets/images/puzzles/`:
- Each set: `puzzle_N_full.png`, `puzzle_N_missing.png`, `prop_N_A.png`, `prop_N_B.png`, `prop_N_C.png`

Implement `initEnigme2(SDL_Renderer *ren)`:
- Load all 5 sets into an array of `PuzzleSet` structs (declared in enigme2.h):
```c
typedef struct {
    SDL_Texture *imgMissing;
    SDL_Texture *propositions[3];
    SDL_Texture *imgFull;
    int          bonneReponse;  /* 0, 1, or 2 */
    int          dejaVue;
} PuzzleSet;
```

### Task 6.2 — Random generation (1 pt)
`genererPuzzle` stub handles no-repeat cycle.
Additionally: randomly shuffle the order of the 3 proposition images each time
(swap indices with Fisher-Yates shuffle on a temp array).

### Task 6.3 — Display puzzle + 3 propositions (1.5 pts)
Implement `afficherEnigme2(SDL_Renderer *ren, PuzzleSet *p)`:
- Left side: render `p->imgMissing` (the incomplete image, 300x300px)
- Right side: render 3 proposition images stacked vertically (150x150px each)
- Highlight the proposition under the mouse cursor (tint with `SDL_SetTextureColorMod`)
- Show a "?" label where the missing piece should go

### Task 6.4 — Mouse drag resolution (1.5 pts)
Implement `resolveEnigme2(Joueur *j, SDL_Event *e, PuzzleSet *p, SDL_Rect *dropZone)`:
- Track mouse button down: start dragging the clicked proposition image
- On mouse motion while dragging: render the image "attached" to the cursor
- On mouse button up: check if release position is inside `dropZone` (the missing-piece area)
  - If correct proposition dropped in zone: success
  - If wrong proposition or wrong zone: fail
- On success: `ajouterScore(j, 75)` + `backg->niveau = "Jouer"`
- On fail: `perdreVie(j)` + `backg->niveau = "Jouer"`

### Task 6.5 — Countdown animation (1.5 pts)
Implement `initChronometreP(int duree)` and `afficherChronometreP(SDL_Renderer *ren)`:
- Use a different visual style from Lot 5 (e.g., a horizontal depleting bar with pulsing color)
- On timeout: auto-fail → `perdreVie(j)` + `backg->niveau = "Jouer"`

### Task 6.6 — Rotozoom success/fail messages (1.5 pts)
Implement `afficherRoutozoom(SDL_Renderer *ren, SDL_Texture *msg, int success)`:
- Load two message images: `msg_succes.png` and `msg_echec.png`
- Animate using `SDL_RenderCopyEx` with an `angle` that increases 5° per frame (0→360)
- Simultaneously scale from 0.1 → 1.0 using `SDL_Rect` size interpolation
- Play for 60 frames then return to game

---

## Interfaces with other lots

| Who calls you | What they need |
|---|---|
| `main.c` | `initEnigme2`, `afficherEnigme2`, `genererPuzzle`, `resolveEnigme2` |
| Lot 2 | You SET `backg->niveau = "Enigme"` / `"Jouer"` |
| Lot 1 | You call `ajouterScore(&joueur, pts)` and `perdreVie(&joueur)` |
| Lot 5 | Coordinate on who sets `backg->niveau` to avoid conflicts |

---

## Include & compile

```c
#include "../common.h"
#include "enigme2.h"
extern Background *backg;
```

---

## Git workflow

```bash
git checkout -b feature/lot6-enigme2
git add lot6_enigme2/enigme2.h lot6_enigme2/enigme2.c
git commit -m "lot6: task 6.1 — initEnigme2 + PuzzleSet loading"
git push origin feature/lot6-enigme2
```
