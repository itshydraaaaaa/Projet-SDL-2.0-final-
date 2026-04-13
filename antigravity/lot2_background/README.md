# Lot 2 — Background Module

**Branch:** `feature/lot2-background`
**Total points:** 8 pts
**Files you own:** `background.h`, `background.c`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

This lot is **fully implemented**. Your responsibilities are:

1. **Keep background.c up to date** as the rest of the team develops their lots.
2. **Provide the `backg` global** to all other lots (declared `extern Background *backg` in background.h).
3. **Review scrolling logic** — make sure `mapW` and `mapH` match the actual image dimensions
   once assets are provided.

---

## Module overview

| Function | Description |
|---|---|
| `initBackground` | Allocates `backg`, loads level textures, sets initial state |
| `initPlateforme` | Sets up a single platform with type, position, velocity, bounds |
| `initTimer` | Records `SDL_GetTicks()` as the start of the session |
| `initGuide` | Pre-renders key-binding help text into textures |
| `initSousMenuScores` | Reads `scores.dat` and pre-renders top-5 score lines |
| `afficherBackground` | Renders BG in single-screen (mode 1) or split-screen (mode 2) |
| `afficherPlateformes` | Renders all active platforms with scroll offset |
| `afficherTemps` | Renders `MM:SS` timer; flashes red in the last 10 s |
| `afficherGuide` | Renders semi-transparent overlay with key bindings |
| `afficherSousMenuScores` | Renders top-5 scores panel |
| `updateScrolling` | Adjusts `offsetX/Y` to keep the player centred in view |
| `updatePlateformesMobiles` | Moves MOBILE platforms and bounces them at boundaries |
| `updateTimer` | Updates `backg->elapsedSec` from `SDL_GetTicks` |
| `detruirePlateforme` | Marks a DESTRUCTIBLE platform as inactive |
| `sauvegarderScore` | Appends score to `scores.dat`, keeps top 5 sorted |
| `chargerScores` | Reads up to 5 records from `scores.dat` |
| `saisirNomJoueur` | Interactive event loop for player name entry |
| `libererBackground` | Destroys all textures and frees `backg` |
| `libererPlateformes` | Destroys platform textures |
| `libererGuide` | Destroys guide overlay textures |

---

## Scrolling rules (exact)

```
if playerX < screenW/2         → offsetX = 0
elif playerX < mapW - screenW/2 → offsetX = playerX - screenW/2
else                            → offsetX = mapW - screenW
clamp: offsetX = SDL_max(0, SDL_min(offsetX, mapW - screenW))
(same logic vertically)
```

---

## Split-screen rendering (mode 2)

```
Left  viewport: SDL_RenderSetViewport(ren, &{0, 0, screenW/2, screenH})
Right viewport: SDL_RenderSetViewport(ren, &{screenW/2, 0, screenW/2, screenH})
Separator: 2px white vertical line at x = screenW/2
```

---

## Interfaces with other lots

| Who calls you | What they use |
|---|---|
| `main.c` | `initBackground`, `afficherBackground`, `updateScrolling`, `updateTimer`, `libererBackground` |
| All lots | `extern Background *backg` — `backg->offsetX/Y`, `backg->niveau` |
| Lot 5 & 6 | Write to `backg->niveau`, `backg->limiteSec` |

---

## Git workflow

```bash
git checkout -b feature/lot2-background
git add lot2_background/background.h lot2_background/background.c
git commit -m "lot2: full implementation — background, scrolling, timer, scores"
git push origin feature/lot2-background
```
