# Antigravity — Assets Guide & Ubuntu Setup

> Complete reference for all required asset files and how to build/run the game on Ubuntu.

---

## 📁 Full Asset Checklist

Place every file listed below in the correct `assets/` subfolder **before** compiling.
The game will fall back to solid-color rectangles if an image is missing, but sound and
fonts will cause errors if absent.

---

### 🖼️ `assets/images/`

These are all the PNG images the game needs.

| Filename | Size (recommended) | Used by | Description |
|---|---|---|---|
| `bg_level1.png` | 2400 × 600 px | Lot 2 | Level 1 scrolling background (wider than the window) |
| `bg_level2.png` | 2400 × 600 px | Lot 2 | Level 2 scrolling background |
| `platform_fixe.png` | 200 × 20 px | Lot 2 | Texture for fixed platforms |
| `platform_mobile.png` | 160 × 20 px | Lot 2 | Texture for moving platforms |
| `platform_destructible.png` | 140 × 20 px | Lot 2 | Texture for breakable platforms |

#### Lot 1 — Player sprites (`assets/images/player/`)

| Filename | Size | Description |
|---|---|---|
| `player_idle.png` | 48 × 64 px | Player standing still (single frame or sprite sheet) |
| `player_walk_1.png` … `player_walk_N.png` | 48 × 64 px | Walking animation frames (up to 8) |
| `player_jump.png` | 48 × 64 px | Jump frame |
| `player_attack.png` | 48 × 64 px | Attack frame |
| `player2_idle.png` | 48 × 64 px | Player 2 idle sprite (different color/skin) |

#### Lot 3 — NPC / Enemy sprites (`assets/images/npc/`)

| Filename | Size | Description |
|---|---|---|
| `enemy_walk_1.png` … `enemy_walk_4.png` | 48 × 64 px | Enemy walk cycle (4 frames) |
| `enemy_attack.png` | 48 × 64 px | Enemy attack frame |
| `collectible_coin.png` | 32 × 32 px | Score collectible entity |
| `collectible_heart.png` | 32 × 32 px | Life collectible entity |

#### Lot 4 — Destruction animation (`assets/images/fx/`)

| Filename | Size | Description |
|---|---|---|
| `explosion_1.png` … `explosion_6.png` | 64 × 64 px | Platform destruction smoke/particle frames |

#### Lot 6 — Puzzle images (`assets/images/puzzles/`)

For each puzzle set **N** (1 to 5), you need 5 files:

| Filename pattern | Size | Description |
|---|---|---|
| `puzzle_N_full.png` | 300 × 300 px | Complete puzzle image (shown on success) |
| `puzzle_N_missing.png` | 300 × 300 px | Same image with a piece hidden / blacked out |
| `prop_N_A.png` | 150 × 150 px | Proposition A (one is correct) |
| `prop_N_B.png` | 150 × 150 px | Proposition B |
| `prop_N_C.png` | 150 × 150 px | Proposition C |

Example for puzzle set 1:
```
assets/images/puzzles/puzzle_1_full.png
assets/images/puzzles/puzzle_1_missing.png
assets/images/puzzles/prop_1_A.png
assets/images/puzzles/prop_1_B.png
assets/images/puzzles/prop_1_C.png
```

#### Lot 6 — Result messages (`assets/images/`)

| Filename | Size | Description |
|---|---|---|
| `msg_succes.png` | 400 × 100 px | "Bravo !" success message (rotozoom animation) |
| `msg_echec.png` | 400 × 100 px | "Perdu !" fail message (rotozoom animation) |

#### Menu / UI (`assets/images/ui/`)

| Filename | Size | Description |
|---|---|---|
| `btn_jouer.png` | 200 × 60 px | "Jouer" menu button (normal state) |
| `btn_jouer_hover.png` | 200 × 60 px | "Jouer" button hover state |
| `btn_options.png` | 200 × 60 px | "Options" button |
| `btn_quitter.png` | 200 × 60 px | "Quitter" button |
| `bg_menu.png` | 800 × 600 px | Main menu background (can equal bg_level1.png) |

---

### 🔊 `assets/sounds/`

| Filename | Format | Used by | Description |
|---|---|---|---|
| `bg_music.ogg` | OGG Vorbis | main.c | Background music loop (plays during "Jouer") |
| `menu_music.ogg` | OGG Vorbis | main.c | Music during main menu (optional) |
| `click.wav` | WAV (16-bit) | All lots | Button click / UI interaction sound |
| `jump.wav` | WAV | Lot 1 | Player jump sound |
| `hurt.wav` | WAV | Lot 1 | Player takes damage |
| `enemy_hit.wav` | WAV | Lot 3 | Enemy takes damage |
| `enemy_die.wav` | WAV | Lot 3 | Enemy dies / neutralized |
| `collect.wav` | WAV | Lot 3 | Pickup a coin or heart |
| `platform_break.wav` | WAV | Lot 2 / Lot 4 | Destructible platform breaks |
| `enigme_correct.wav` | WAV | Lot 5 & 6 | Correct answer sound |
| `enigme_wrong.wav` | WAV | Lot 5 & 6 | Wrong answer sound |
| `save.wav` | WAV | Lot 4 | Game saved confirmation |

> **Format rules:**
> - Background music → `.ogg` (smaller file, loops cleanly with `Mix_PlayMusic`)
> - Sound effects → `.wav` (lowest latency, use `Mix_PlayChannel`)

---

### 🔤 `assets/fonts/`

| Filename | Used by | Description |
|---|---|---|
| `game_font.ttf` | All lots | Main game font — used for timer, scores, HUD, menus |

> **Recommended free fonts** (pick one, rename it to `game_font.ttf`):
> - [Press Start 2P](https://fonts.google.com/specimen/Press+Start+2P) — retro pixel style
> - [Orbitron](https://fonts.google.com/specimen/Orbitron) — sci-fi / futuristic
> - [Roboto](https://fonts.google.com/specimen/Roboto) — clean and readable
>
> Download from [Google Fonts](https://fonts.google.com) and place as `assets/fonts/game_font.ttf`.

---

## 🐧 Running on Ubuntu — Step by Step

### 1. Install dependencies

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev
```

Verify the libraries are found:

```bash
sdl2-config --version        # should print e.g. 2.0.20
pkg-config --modversion SDL2_image SDL2_ttf SDL2_mixer
```

---

### 2. Get the project

If you already have the folder (e.g. copied from Windows or cloned from GitHub):

```bash
# Navigate to the project root
cd ~/antigravity          # adjust path as needed
```

If cloning from GitHub:

```bash
git clone https://github.com/YOUR_USERNAME/antigravity.git
cd antigravity
```

---

### 3. Add your assets

Copy your PNG / OGG / WAV / TTF files into the correct subfolders:

```bash
# Example: copy a font
cp ~/Downloads/PressStart2P-Regular.ttf assets/fonts/game_font.ttf

# Example: copy background images
cp ~/Downloads/bg_level1.png assets/images/bg_level1.png
cp ~/Downloads/bg_level2.png assets/images/bg_level2.png
```

The game can start **without** images (falls back to colored rectangles) but
**requires** a font file for any text rendering.

---

### 4. Compile

```bash
make
```

Expected output (no errors):

```
gcc -Wall -Wextra -std=c99 -I. -c main.c -o main.o
gcc -Wall -Wextra -std=c99 -I. -c lot1_joueur/joueur.c -o lot1_joueur/joueur.o
gcc -Wall -Wextra -std=c99 -I. -c lot2_background/background.c -o lot2_background/background.o
...
gcc *.o lot*/*.o -o antigravity -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
```

If you get errors, see the **Troubleshooting** section below.

---

### 5. Run

```bash
./antigravity
```

---

### 6. Clean build files

```bash
make clean
```

---

## 🔧 Troubleshooting on Ubuntu

### ❌ `fatal error: SDL2/SDL.h: No such file or directory`

The SDL2 development headers are missing:

```bash
sudo apt install libsdl2-dev
```

---

### ❌ `cannot find -lSDL2_image` (or `_ttf`, `_mixer`)

```bash
sudo apt install libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

---

### ❌ `TTF_OpenFont warning: Couldn't open font`

The font file is missing. Add it:

```bash
ls assets/fonts/          # should list game_font.ttf
```

Download a font and copy it there (see the Fonts section above).

---

### ❌ `IMG_LoadTexture: Couldn't open assets/images/bg_level1.png`

This is a **warning**, not a fatal error — the game still runs with a dark blue fallback.
Add your PNG files to `assets/images/` when ready.

---

### ❌ Linker error: `undefined reference to 'sqrt'`

Add `-lm` to the Makefile `LIBS` line (it is already included in the provided Makefile):

```makefile
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
```

---

### ❌ `error while loading shared libraries: libSDL2-2.0.so.0`

SDL2 runtime library not found. Fix with:

```bash
sudo ldconfig
# or
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

---

## 📋 Quick reference — Asset folder layout

```
antigravity/
└── assets/
    ├── fonts/
    │   └── game_font.ttf               ← REQUIRED
    ├── images/
    │   ├── bg_level1.png               ← REQUIRED for Level 1
    │   ├── bg_level2.png               ← REQUIRED for Level 2
    │   ├── platform_fixe.png
    │   ├── platform_mobile.png
    │   ├── platform_destructible.png
    │   ├── msg_succes.png              ← Lot 6
    │   ├── msg_echec.png               ← Lot 6
    │   ├── bg_menu.png                 ← Menu background
    │   ├── player/
    │   │   ├── player_idle.png
    │   │   ├── player_walk_1.png … player_walk_N.png
    │   │   ├── player_jump.png
    │   │   └── player_attack.png
    │   ├── npc/
    │   │   ├── enemy_walk_1.png … enemy_walk_4.png
    │   │   ├── enemy_attack.png
    │   │   ├── collectible_coin.png
    │   │   └── collectible_heart.png
    │   ├── fx/
    │   │   └── explosion_1.png … explosion_6.png
    │   ├── puzzles/
    │   │   ├── puzzle_1_full.png … puzzle_5_full.png
    │   │   ├── puzzle_1_missing.png … puzzle_5_missing.png
    │   │   ├── prop_1_A.png … prop_5_C.png
    │   └── ui/
    │       ├── btn_jouer.png
    │       ├── btn_jouer_hover.png
    │       ├── btn_options.png
    │       └── btn_quitter.png
    └── sounds/
        ├── bg_music.ogg                ← REQUIRED
        ├── menu_music.ogg
        ├── click.wav
        ├── jump.wav
        ├── hurt.wav
        ├── enemy_hit.wav
        ├── enemy_die.wav
        ├── collect.wav
        ├── platform_break.wav
        ├── enigme_correct.wav
        ├── enigme_wrong.wav
        └── save.wav
```

---

## 🌐 Free asset sources

| Type | Website | Notes |
|---|---|---|
| Sprite sheets | [itch.io/game-assets](https://itch.io/game-assets) | Many free 2D assets |
| Sprite sheets | [opengameart.org](https://opengameart.org) | CC0 / CC-BY licenses |
| Backgrounds | [craftpix.net/freebies](https://craftpix.net/freebies/) | Free 2D game backgrounds |
| Sound FX | [freesound.org](https://freesound.org) | Free with account |
| Sound FX | [mixkit.co/free-sound-effects](https://mixkit.co/free-sound-effects/) | No account needed |
| Music | [freemusicarchive.org](https://freemusicarchive.org) | CC-licensed music |
| Fonts | [fonts.google.com](https://fonts.google.com) | Free OFL fonts |

> ⚠️ Always check the license before using an asset in a university submission.
> CC0 and CC-BY are the safest choices.

---

*Antigravity — ESPRIT 1A — 2020-2021*
