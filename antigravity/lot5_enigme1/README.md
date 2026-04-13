# Lot 5 — Enigme 1 : QCM (Multiple Choice Quiz Module)

**Branch:** `feature/lot5-enigme1`
**Total points:** 8 pts
**Files you own:** `enigme1.h`, `enigme1.c`, `questions.txt`
**Do NOT touch:** `main.c`, `common.h`, or any other lot's files.

---

## What you need to do

Your module manages the QCM (multiple choice) riddle system. When the player collides
with an enigme trigger in the world, `backg->niveau` is set to `"Enigme"` — your module
then takes over display and resolution.

### Task 5.1 — Fill questions.txt (1 pt)
The file `questions.txt` already contains 12 starter questions.
Add at least 10 more relevant to your game theme.
Format: one question per line, fields separated by `|`:

```
Question text here|Answer A|Answer B|Answer C|1
```
Last field is the correct answer index (1, 2, or 3).

### Task 5.2 — Init & display enigme from file (1 pt)
The `Question` struct is declared in `enigme1.h`:
```c
typedef struct {
    char question[256];
    char choix[3][128];
    int  bonneReponse;   /* 1, 2, or 3 */
    int  dejaVue;        /* 0=not seen, 1=already shown */
} Question;
```

`initEnigme1(const char *fichier)` already parses `questions.txt` — check the stub.

Implement `afficherEnigme1(SDL_Renderer *ren, TTF_Font *font, Question *q)`:
- Render the question text at center screen
- Render 3 answer buttons labeled A, B, C
- Highlight hovered button on mouse motion

### Task 5.3 — Random no-repeat generation (1.5 pts)
`genererQuestion` stub already handles the no-repeat cycle logic.
Verify it and complete the display integration.

### Task 5.4 — Resolution + score/life/level management (1.5 pts)
`resolveEnigme1` stub already wires up `ajouterScore`, `perdreVie`, and `backg->limiteSec`.
Add sound effects and complete the integration.

### Task 5.5 — Enigme countdown animation (1.5 pts)
Implement `initChronometre(int duree)` and `afficherChronometre(SDL_Renderer *ren)`:
- Do NOT display a number — display a **graphical arc** that shrinks:
  - Use `SDL_RenderDrawLine` to approximate an arc (or a shrinking bar)
  - Color transitions: green (>20s) → orange (10-20s) → red (<10s)
- When time runs out: call `resolveEnigme1` with a wrong answer automatically

### Task 5.6 — Enigme sub-menu (1.5 pts)
Implement `afficherSousMenuEnigme(SDL_Renderer *ren)`:
- Show question number (randomly chosen between 1 and 10)
- Display a "Tirage au sort" animation (spinning wheel / random number flash)
- Allow choosing enigme type if both Lot 5 and Lot 6 are available (QCM vs Puzzle)

---

## Interfaces with other lots

| Who calls you | What they need |
|---|---|
| `main.c` | `initEnigme1`, `afficherEnigme1`, `genererQuestion`, `resolveEnigme1` |
| Lot 2 | You SET `backg->niveau = "Enigme"` / `"Jouer"` |
| Lot 1 | You call `ajouterScore(&joueur, pts)` and `perdreVie(&joueur)` |

---

## Include & compile

```c
#include "../common.h"
#include "enigme1.h"
extern Background *backg;
```

---

## Git workflow

```bash
git checkout -b feature/lot5-enigme1
git add lot5_enigme1/enigme1.h lot5_enigme1/enigme1.c lot5_enigme1/questions.txt
git commit -m "lot5: task 5.1 — questions.txt with 10 questions"
git push origin feature/lot5-enigme1
```
