# Lot 1 — Joueur (Player Module)

**Branch:** `feature/lot1-joueur`
**Total points:** 8 pts
**Files you own:** `joueur.h`, `joueur.c`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

Your module manages everything related to the player character(s).
The `Joueur` struct is already defined in `common.h` — use it directly, never redefine it.

### Task 1.1 — Init & display player (1 pt)
Implement `initJoueur(Joueur *j, SDL_Renderer *ren)`:
- Load the player sprite image(s) into `j->img[]`
- Set initial position: `j->rect.x = 100`, `j->rect.y = 400`
- Set initial values: `j->vie = 3`, `j->score = 0`, `j->vitesse = 3.0f`

Implement `afficherJoueur(SDL_Renderer *ren, Joueur *j)`:
- Render `j->img[j->frame]` at `j->rect` adjusted by the background offset:
  `SDL_Rect dest = { j->rect.x - backg->offsetX, j->rect.y - backg->offsetY, j->rect.w, j->rect.h };`
- Render score and lives as SDL_ttf text in the top-left corner

### Task 1.2 — Lives & score management (1.5 pts)
Implement `perdreVie(Joueur *j)`:
- Decrement `j->vie`. If `j->vie <= 0` → set game over flag.

Implement `ajouterScore(Joueur *j, int points)`:
- Increment `j->score += points`

These are called from `main.c` — keep their signatures exactly as declared in `joueur.h`.

### Task 1.3 — Animate player (1 pt)
Implement `animerJoueur(Joueur *j)`:
- Cycle through `j->img[0..N]` using a frame counter
- Advance frame every ~8 game ticks (use a static counter inside the function)
- Reset to frame 0 when reaching the last frame

### Task 1.4 — Move player (2 pts)
Implement `deplacerJoueur(Joueur *j, const char *input)`:
- `"left"`  → `j->rect.x -= j->vitesse` + set `j->direction = "left"`
- `"right"` → `j->rect.x += j->vitesse` + set `j->direction = "right"`
- `"up"` / space → trigger jump: `j->jumping = 1`, apply upward velocity
- `"attack"` → play attack animation (switch to attack sprite frame)
- Implement basic gravity: if `j->jumping`, apply `vy` each frame, stop when player lands on a platform

### Task 1.5 — Second player (1 pt)
Implement `initJoueur2(Joueur *j2, SDL_Renderer *ren)`:
- Same as `initJoueur` but starting position is different (e.g. x=650)

Call `afficherJoueur` and `deplacerJoueur` twice in `main.c` — once per Joueur instance.
The split-screen rendering for player 2 is handled by Lot 2 (`afficherBackground(ren, 2)`).

### Task 1.6 — Character selection sub-menu (1.5 pts)
Implement `initSousMenuJoueur(SDL_Renderer *ren)` and `afficherSousMenuJoueur(SDL_Renderer *ren)`:
- Display 2–3 selectable character images with mouse hover highlight
- Player clicks to select → loads the corresponding sprite set
- Also allow outfit/skin selection (swap `img[]` textures)
- Triggered when `backg->niveau == "Menu"` and player clicks a "Choose Character" button

---

## Interfaces with other lots

| Who calls you | What they need |
|---|---|
| `main.c` | `initJoueur`, `afficherJoueur`, `deplacerJoueur`, `perdreVie`, `ajouterScore` |
| Lot 2 | Your `j->rect.x/y` for scrolling — pass by pointer via `main.c` |
| Lot 3 | Your `j->rect` for bounding-box collision |
| Lot 4 | Your `j->rect.x/y` for mini-map position dot |

---

## Include & compile

```c
#include "../common.h"
#include "joueur.h"
// Access background offsets:
extern Background *backg;  // defined in lot2_background/background.c
```

Compile test: `gcc -Wall -c lot1_joueur/joueur.c -I. $(sdl2-config --cflags)` — must produce 0 errors.

---

## Git workflow

```bash
git checkout -b feature/lot1-joueur
# ... write your code ...
git add lot1_joueur/joueur.h lot1_joueur/joueur.c
git commit -m "lot1: task 1.1 — initJoueur + afficherJoueur"
git push origin feature/lot1-joueur
# Open a Pull Request → main when all 6 tasks are done
```
