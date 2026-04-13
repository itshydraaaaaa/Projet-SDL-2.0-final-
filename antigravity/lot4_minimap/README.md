# Lot 4 — Mini map (Minimap + Collisions + Save/Load)

**Branch:** `feature/lot4-minimap`
**Total points:** 8 pts
**Files you own:** `minimap.h`, `minimap.c`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

Your module draws the mini-map overlay and handles advanced collisions and the save/load sub-menu.

### Task 4.1 — Init & display mini-map (1 pt)
Implement `initMiniMap(SDL_Renderer *ren)`:
- Define the mini-map area: e.g. bottom-right corner, 160x90px
- Scale factor: `scaleX = 160.0f / backg->mapW`, `scaleY = 90.0f / backg->mapH`

Implement `afficherMiniMap(SDL_Renderer *ren, Joueur *j, NPC *ennemis, int nbEnnemis)`:
- Draw a filled dark rectangle as mini-map background
- Draw a white dot for the player position: `(j->rect.x * scaleX, j->rect.y * scaleY)`
- Draw red dots for each active enemy
- Draw a green rectangle showing the current viewport (camera position)

Generate for BOTH Level 1 and Level 2 — use `backg->niveau` to detect which one.

### Task 4.2 — Auto mini-map update (1 pt)
The mini-map auto-updates because it reads live positions from `j->rect` and `e->rect`.
Implement `majMiniMap(Joueur *j1, Joueur *j2)`:
- In split-screen mode, show BOTH players as different colored dots (white + yellow)
- Called every frame from `main.c` just before `afficherMiniMap`

### Task 4.3 — Perfect-pixel collision with background (1.5 pts)
Implement `int collisionPixelParfait(SDL_Texture *bg, SDL_Rect *rect, int offsetX, int offsetY)`:
- Lock the background surface (you need `SDL_Surface*` version of the BG, keep it alongside the texture)
- For each pixel in `rect` that overlaps the background (accounting for offset):
  - Read pixel alpha: `SDL_GetRGBA(...)`
  - If alpha > 128 → collision detected, return 1
- Return 0 if no solid pixel found

This is expensive — call it only when bounding-box already detected overlap.

### Task 4.4 — BB collision with mobile platforms (1.5 pts)
Implement `int collisionPlatformeMobile(SDL_Rect *joueur, Plateforme *p)`:
- Use `detectionBB` from Lot 3 (include `"../lot3_npc/npc.h"` or declare it extern)
- If collision from above → land player on top of platform: `joueur->y = p->rect.y - joueur->h`
- If collision from side → push player back: `joueur->x -= p->dx`
- Return 1 if collision occurred

### Task 4.5 — Animated background collision (1.5 pts)
Implement `animerCollisionBackground(SDL_Renderer *ren, int x, int y)`:
- When a destructible platform is destroyed (Lot 2 sets `p->active = 0`), play a short particle/smoke animation at position (x, y)
- Use a sprite sheet of 4–6 frames, advance frame every 3 ticks, then stop
- Store animation state in a small struct: `{ int frame; int active; SDL_Rect pos; }`

### Task 4.6 — Save & load sub-menu (1.5 pts)
Implement `initSousMenuSauvegarde(SDL_Renderer *ren)` and `afficherSousMenuSauvegarde(SDL_Renderer *ren)`:
- Display two buttons: "Sauvegarder" and "Charger"
- Saving: write current `Joueur` state (score, vie, position, niveau) to `save.dat` with `fwrite`
- Loading: `fread` back into `Joueur` and restore `backg->niveau`
- Show a confirmation message after save/load ("Jeu sauvegardé !")

---

## Interfaces with other lots

| Who calls you | What they need |
|---|---|
| `main.c` | `initMiniMap`, `afficherMiniMap`, `majMiniMap`, `collisionPlatformeMobile` |
| Lot 2 | Read `backg->offsetX/Y`, `backg->mapW/H`, platform array |
| Lot 1 | Read `j->rect.x/y` for player dot on map |
| Lot 3 | Read `e->rect.x/y` for enemy dots; use `detectionBB` |

---

## Include & compile

```c
#include "../common.h"
#include "minimap.h"
extern Background *backg;
```

---

## Git workflow

```bash
git checkout -b feature/lot4-minimap
git add lot4_minimap/minimap.h lot4_minimap/minimap.c
git commit -m "lot4: task 4.1 — initMiniMap + afficherMiniMap"
git push origin feature/lot4-minimap
```
