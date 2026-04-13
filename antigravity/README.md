# Antigravity — ESPRIT Projet 1A (2020-2021)

A 2D side-scrolling serious game in C with SDL2.

## Team lot assignments

| Lot | Module | Branch | Owner |
|-----|--------|--------|-------|
| Lot 1 | `lot1_joueur/` — Player | `feature/lot1-joueur` | |
| **Lot 2** | `lot2_background/` — **Background** | `feature/lot2-background` | **YOU** |
| Lot 3 | `lot3_npc/` — Enemies | `feature/lot3-npc` | |
| Lot 4 | `lot4_minimap/` — Minimap | `feature/lot4-minimap` | |
| Lot 5 | `lot5_enigme1/` — QCM Riddles | `feature/lot5-enigme1` | |
| Lot 6 | `lot6_enigme2/` — Puzzle Riddles | `feature/lot6-enigme2` | |

## Build

```bash
# Linux / WSL
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
make
./antigravity

# macOS (Homebrew)
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
make
./antigravity
```

## Rules
1. Each student works ONLY in their lot folder.
2. Shared types go in `common.h` only — never duplicated.
3. PRs target `main`. Must compile with 0 errors before merge.
4. Commit message format: `lotN: short description`
5. Place asset files in `assets/` — never hardcode absolute paths.

## Game loop states

```
backg->niveau values:
  "Menu"    → show main menu buttons
  "Options" → show volume/display buttons
  "Jouer"   → show player + enemies + platforms
  "Enigme"  → show QCM or Puzzle riddle screen
```

## Folder structure

```
antigravity/
├── main.c
├── common.h
├── Makefile
├── scores.dat              (created at runtime)
├── assets/
│   ├── images/             (provide PNG files here)
│   ├── sounds/             (provide OGG/WAV files here)
│   └── fonts/              (provide TTF files here)
├── lot1_joueur/
├── lot2_background/        ← FULLY IMPLEMENTED
├── lot3_npc/
├── lot4_minimap/
├── lot5_enigme1/
└── lot6_enigme2/
```

## Asset checklist

Place these files in `assets/` before compiling:

| File | Used by |
|---|---|
| `assets/images/bg_level1.png` | Lot 2 — Level 1 background |
| `assets/images/bg_level2.png` | Lot 2 — Level 2 background |
| `assets/images/platform_fixe.png` | Lot 2 — Fixed platform |
| `assets/images/platform_mobile.png` | Lot 2 — Moving platform |
| `assets/images/platform_destructible.png` | Lot 2 — Breakable platform |
| `assets/sounds/bg_music.ogg` | main.c — Background music |
| `assets/sounds/click.wav` | Buttons |
| `assets/fonts/game_font.ttf` | All text rendering |
