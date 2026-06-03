# Missile Game

A 2D artillery game written in C++ on top of the [Allegro 5](https://liballeg.org/)
game library. Drag a missile back in the launcher, aim, and let it fly across a
physics-driven arc to smash the targets hidden among the blocks.

This was a first-year programming project for the **Programmeren 2020** course at
**Ghent University (UGent)**, built by Robbe Delporte and Pieter Lecompte. It is
archived here as-is; the code is preserved as it was at the end of the course.

> **Note on the build.** The project was originally developed from VS Code on
> WSL using a hand-written `g++` task. This repo replaces that editor-specific
> setup with a portable `Makefile` so it builds and runs from the command line
> on any Linux distribution (and on Windows via WSL).


<img width="900" height="450" alt="image" src="https://github.com/user-attachments/assets/21f843ad-2495-4b1e-8be4-1bdf257620f2" />

## Gameplay

- Pick a level from the menu (three handcrafted levels are included).
- Each level is an 8×8 grid of blocks. Some blocks are plain bricks, some are
  tougher stone/obsidian, and some hide **targets** you have to destroy.
- You have a limited number of **missiles** with different behaviours. Grab one
  from the launcher with the mouse, drag back to set the angle and power
  (slingshot style), and release to launch. Gravity and friction shape the arc.
- Some missiles have a **special action** you can trigger mid-flight.
- Clear all targets to win. Your time is recorded, and the best runs are saved
  as highscores you can replay from the menu.

### Controls

| Context  | Input                                   | Action                                  |
| -------- | --------------------------------------- | --------------------------------------- |
| Menus    | Arrow Up / Down                         | Move between buttons                    |
| Menus    | Enter                                   | Select                                  |
| In-game  | Mouse press + drag on a launcher missile| Aim and set power                       |
| In-game  | Release mouse                           | Launch the missile                      |
| In-game  | Spacebar                                | Trigger the active missile's special    |
| Any      | Close the window                        | Quit                                    |

## Architecture

The game uses a small **Entity–Component–System (ECS)** design:

- **Entities** (`Entity`, `EntityStream`) are bare containers identified by the
  components they hold.
- **Components** (`*Component.h`, e.g. `PositionComponent`, `MissileQueueComponent`,
  `TargetComponent`) are plain data.
- **Systems** (`LauncherSystem`, `MissileSystem`, `TargetSystem`, `LevelSystem`,
  `RenderSystem`) hold the logic and operate on entities that have the components
  they care about. The `Engine` ticks the systems each frame.
- **`Allkit`** is a thin singleton wrapper around Allegro that owns the display,
  event queue, fonts, sounds and sprites, and exposes simple input/draw helpers
  so the rest of the code never touches Allegro directly.

`Game` runs the in-level loop, `Menu` drives the menus, and `Constants.h` holds
the tunable values (screen size, FPS, physics constants, etc.).

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- `make`
- **Allegro 5** with these addons: `audio`, `acodec`, `dialog`, `font`, `image`,
  `primitives`, `ttf`.

Install Allegro with your package manager:

```sh
# Arch Linux
sudo pacman -S allegro

# Debian / Ubuntu
sudo apt install liballegro5-dev liballegro-acodec5-dev liballegro-audio5-dev \
                 liballegro-image5-dev liballegro-dialog5-dev liballegro-ttf5-dev

# Fedora
sudo dnf install allegro5-devel 'allegro5-addon-*-devel'
```

The `Makefile` locates Allegro through `pkg-config`, so any standard install of
the development packages is picked up automatically.

## Build and run

```sh
make            # build the optimised release binary (./missile-game)
make run        # build if needed, then run from the project root
make debug      # build an unoptimised binary with debug symbols (for gdb)
make clean      # remove all build artefacts
```

The game loads its assets through relative `./assets/...` paths, so it **must be
run from the project root**. `make run` always does that for you; if you launch
the binary by hand, do it from the repository root (`./missile-game`).

### Running under WSL

On WSL there is no display or audio server by default, so the game detects WSL at
startup and points itself at one running on the Windows host. Before launching:

1. Start an X server on Windows (for example [VcXsrv](https://sourceforge.net/projects/vcxsrv/) / XLaunch).
2. Start a PulseAudio server reachable at `127.0.0.1:4713`.

If either is missing the game prints an explanatory error and exits. On a native
Linux desktop this step is skipped automatically.

## Project layout

```
.
├── Main.cpp              # entry point + WSL display/audio bootstrap
├── Allkit.{h,cpp}        # Allegro wrapper (display, input, audio, drawing)
├── Engine.{h,cpp}        # ECS engine, ticks the systems
├── Entity*.{h,cpp}       # entities and the entity stream
├── *Component.h          # ECS components (data)
├── *System.{h,cpp}       # ECS systems (logic)
├── Game.{h,cpp}          # in-level game loop
├── Menu.{h,cpp}          # menus (start / level select / replay)
├── Constants.h           # screen, FPS and physics tuning
├── Makefile              # build (Linux / WSL)
└── assets/               # images, fonts, sounds, levels, highscores
```

## Authors

- Robbe Delporte
- Pieter Lecompte

Created in 2020 for *Programmeren* at Ghent University.
