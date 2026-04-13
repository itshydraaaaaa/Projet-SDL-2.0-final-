# Lot 3 — NPC / Ennemi (Enemy Module)

**Branch:** `feature/lot3-npc`
**Total points:** 8 pts
**Files you own:** `npc.h`, `npc.c`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

Your module manages all non-player characters: enemies and collectible entities (coins, hearts…).
The `NPC` struct is in `common.h`. Use it directly.

### Task 3.1 — Init & display NPCs (1 pt)
Implement `initNPC(NPC *e, SDL_Renderer *ren, int x, int y)`:
- Load enemy sprite frames into `e->img[]`
- Set `e->rect`, `e->vie = 3`, `e->active = 1`, `e->vitesse = 1.5f`

Implement `afficherNPC(SDL_Renderer *ren, NPC *e)`:
- Render `e->img[e->frame]` offset by `backg->offsetX / offsetY` (same as player rendering)
- Do not render if `e->active == 0`

Call both for Level1 enemies AND Level2 enemies (you manage two arrays or a flag).

### Task 3.2 — Random movement (1 pt)
Implement `deplacerNPCAléatoire(NPC *e)`:
- Two predefined trajectories per level (e.g. horizontal patrol / diagonal bounce)
- Use a direction variable: flip when hitting a boundary or after N frames
- Use `rand()` seeded once in `main.c` with `srand(SDL_GetTicks())`

### Task 3.3 — Enemy animation (1 pt)
Implement `animerNPC(NPC *e)`:
- Cycle through walk frames when moving
- Switch to attack frames when `e->direction == "attack"`
- Use a static frame counter, reset on state change

### Task 3.4 — Bounding-box collision (1.5 pts)
Implement ONE generic function: `int detectionBB(SDL_Rect *a, SDL_Rect *b)`:
- Returns 1 if the two rectangles overlap, 0 otherwise
- Formula: `!(a->x+a->w < b->x || b->x+b->w < a->x || a->y+a->h < b->y || b->y+b->h < a->y)`

Call it twice from `main.c`:
1. `detectionBB(&joueur.rect, &ennemi.rect)` → on collision: `perdreVie(&joueur)`
2. `detectionBB(&joueur.rect, &entiteScore.rect)` → on collision: `ajouterScore(&joueur, 10)`

### Task 3.5 — Health system (1.5 pts)
Add `sante` field tracking: vivant (3 vie), blessé (1-2 vie), neutralisé (0 vie).

Implement `afficherSanteNPC(SDL_Renderer *ren, NPC *e)`:
- Draw a colored bar above the NPC: green (full) → orange (half) → red (low)
- Bar width proportional to `e->vie / max_vie`

Implement `blesserNPC(NPC *e, int degats)`:
- Decrement `e->vie -= degats`
- If `e->vie <= 0` → `e->active = 0` (neutralized)
- Switch to "blessé" sprite if `e->vie == 1`

### Task 3.6 — AI automatic movement (2 pts)
Implement `deplacerNPCAI(NPC *e, Joueur *j)`:
- If distance to player < 200px → chase the player (move toward j->rect.x/y)
- If distance > 200px → patrol (random trajectory from task 3.2)
- Add a basic line-of-sight check: only chase if on the same Y level (±50px)
- Distance formula: `sqrt(pow(e->rect.x - j->rect.x, 2) + pow(e->rect.y - j->rect.y, 2))`

---

## Interfaces with other lots

| Who calls you | What they need |
|---|---|
| `main.c` | `initNPC`, `afficherNPC`, `deplacerNPCAI`, `blesserNPC`, `animerNPC` |
| Lot 1 | Provide `detectionBB` — called with `&joueur.rect` |
| Lot 4 | Your `e->rect.x/y` for mini-map enemy dot positions |

---

## Include & compile

```c
#include "../common.h"
#include "npc.h"
extern Background *backg;
```

---

## Git workflow

```bash
git checkout -b feature/lot3-npc
git add lot3_npc/npc.h lot3_npc/npc.c
git commit -m "lot3: task 3.1 — initNPC + afficherNPC Level1"
git push origin feature/lot3-npc
```
