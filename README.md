# Pac-Man Game Proposal

**Team Lead:** Rayan Khan (26L-2633)

**Team Members:**
- Muhammad Abdullah (26L-2554)
- Muhammad Hamza (26L-2556)
- Khizar Rehman (26L-2574)

**Date:** October 18, 2026

---

# Pac-Man Game - Project Documentation

## 1. Project overview

This project is a 2D Pac-Man-style maze game written in C++ using SFML 3. The player moves through a 25 by 25 tile maze, collects pellets and bonus items, avoids or defeats ghosts, and progresses through increasingly difficult levels. The game also includes timed power-ups, a frenzy mode, teleport passages, sound effects, achievements, and two separate leaderboards.

The game is rendered in a 1500 by 900 window. A 900 by 900 maze occupies the centre, an information panel appears on the right, and achievement/progress panels appear on the left.

## 2. Main features

- Grid-locked Pac-Man movement using WASD or the arrow keys.
- Wall collision checks based on the player's bounding-box corners.
- Pellets worth 10 points, with a 300-second respawn timer.
- Four power-up orbs: two speed orbs, one invincibility orb, and one ghost-killing orb.
- Power-up effects last 10 seconds; collected power-up orbs respawn after 60 seconds.
- Frenzy mode: every 60 seconds of active play, scoring is multiplied by five for 10 seconds and ghosts leave the house faster.
- Four ghost types with different target-selection behaviours, followed by up to four additional ghosts in later levels.
- Chase and scatter movement phases, ghost release delays, and boss mode when only one ghost remains alive.
- A grape collectible that grants an extra life, and a separate orange bonus orb worth 1,000 points.
- Entry and exit gates, level progression, and a 10% speed increase per completed level.
- A player name-entry screen, pause screen, and game-over screen.
- Pellet, chase-warning, death, and menu audio.
- Five persistent achievements, a top-three progress board, and a top-five score leaderboard.

## 3. Controls

| Input | Action |
|---|---|
| W or Up Arrow | Request movement upward |
| A or Left Arrow | Request movement left |
| S or Down Arrow | Request movement downward |
| D or Right Arrow | Request movement right |
| P or Escape | Pause or resume |
| Space | Restart after game over |
| Enter | Confirm the player name |
| Backspace | Delete the last character on the name-entry screen |

Movement is grid-locked. A requested turn is normally performed when Pac-Man reaches the centre of a tile and the next tile is open. Reversing direction is allowed immediately.

## 4. Window layout

The game uses an SFML view whose left edge is at x = -300. This makes room for the left-side panels without changing the maze coordinates.

| Screen region | Coordinates | Contents |
|---|---:|---|
| Left panel | x = -300 to 0 | Top-three progress board and achievements |
| Maze | x = 0 to 900; y = 0 to 900 | Maze, Pac-Man, ghosts, pellets, gates, and collectibles |
| Right panel | x = 900 to 1200 | Game title, score, lives, power-up timers, level and gate information |

The window itself is 1500 by 900 pixels. Each maze tile is 36 by 36 pixels, giving a 900 by 900 maze.

## 5. Maze representation

The maze is stored in a one-dimensional integer array named `maze`. A tile's array index is calculated as:

```cpp
index = row * COLS + column;
```

The constants are:

- `CELL = 36`: tile width and height in pixels.
- `COLS = 25`: number of columns.
- `ROWS = 25`: number of rows.

Tile values have the following meanings:

| Value | Meaning |
|---:|---|
| `1` | Wall |
| `0` | Empty walkable tile before pellets are placed, or after a pellet is collected |
| `2` | Pellet currently present |
| `-1` | Special edge/tunnel marker; treated as blocked by ghost path selection, but not as a normal wall by Pac-Man's `isWall` check |

`place_dots()` changes eligible empty tiles (`0`) to pellet tiles (`2`). It deliberately leaves the ghost-house region without pellets. The house exclusion covers rows 9-12 and columns 10-14.

A tile's pixel centre is calculated as:

```cpp
x = (column + 0.5f) * CELL;
y = (row + 0.5f) * CELL;
```

## 6. Game objects and state

### Player

The `Player` structure contains the Pac-Man sprite, remaining lives, score, a state field, and base movement speed. The game starts with one life and a score of -10. The initial negative score offsets the first 10-point pellet on the starting tile, so the displayed score becomes zero after that pellet is collected.

### Ghost

The `Ghost` structure contains a sprite, a state field, movement speed, an `alive` flag, and an `isBoss` flag. The game creates eight ghost objects, but only the first four are active in the first level. The active count increases by one after each completed level, up to eight.

### Ghost movement arrays

The ghost movement system uses parallel arrays for each ghost's current tile (`gr`, `gc`), target/next tile (`nr`, `nc`), movement direction (`vr`, `vc`), home tile (`hr`, `hc`), release delay (`rel`), and scatter corner (`cr`, `cc`). This keeps each ghost's grid-based movement state separate while reusing the same movement loop.

## 7. Gameplay systems

### 7.1 Player movement and collision

Each frame, the game reads the movement keys and stores the requested direction in `wanted`. The current position is converted to a maze row and column. Pac-Man moves along one axis at a time, keeping the other coordinate aligned to the centre of the current row or column.

`canMoveTo()` checks the four corners of a square around the proposed player position. The movement is rejected if any corner is inside a normal wall tile. The collision half-size is set to 15 pixels.

Pac-Man's base speed is 120 pixels per second. The effective speed is multiplied by the active speed power-up multiplier and the current level multiplier.

### 7.2 Pellets and score

The game scans the maze for tiles whose value is `2`. When Pac-Man comes within 15 pixels of a pellet centre:

1. The tile changes from `2` to `0`.
2. A pellet sound is attempted, provided another tracked audio source is not playing.
3. The score increases by 10 times the active score multiplier.
4. The level's collected-pellet counter (`got`) increases.
5. A 300-second respawn timer is assigned to that tile.

The normal score multiplier is 1. During frenzy mode it is 5. A pellet whose timer expires is restored to `2` if its tile is still `0`.

### 7.3 Power-up orbs

There are four fixed power-up positions. The `orbType` array determines each orb's effect.

| Type | Colour | Effect |
|---:|---|---|
| `1` | Cyan | Increases Pac-Man's speed by a factor of 1.6 |
| `2` | Gold | Prevents ghost collisions from costing a life |
| `3` | Red | Allows Pac-Man to eliminate non-boss ghosts |

Each effect lasts 10 seconds. Collecting an orb awards 50 points multiplied by the current frenzy multiplier. The collected orb disappears and respawns after 60 seconds. The speed effect and level speed multiplier are both applied to Pac-Man's movement.

The kill-ghost effect does not eliminate a boss ghost. It also only eliminates a normal ghost when more than one living ghost remains.

### 7.4 Frenzy mode

After the player starts moving, `frenzyWait` counts active gameplay time. After 60 seconds, frenzy mode starts and lasts 10 seconds. During frenzy:

- Pellet, orb, and bonus-orb score awards use a multiplier of 5.
- The maze background is tinted purple.
- Wall glow colours change to pink/purple.
- Ghost release timing advances at twice the normal rate.

The countdowns are updated only while the game is active and not paused.

### 7.5 Ghost AI

Ghosts are released from the house according to their individual release delays. Their release delays are spaced three seconds apart. Ghosts use grid-based movement and choose among available neighbouring tiles by selecting the tile with the smallest squared distance to their current target. They normally cannot reverse direction unless no valid forward choice exists.

The target depends on the ghost type and current movement mode:

- Red ghost: targets Pac-Man's current tile.
- Pink ghost: targets four tiles ahead of Pac-Man's facing direction.
- Blue ghost: targets a point calculated using Pac-Man's position and the red ghost's position.
- Yellow ghost: chases when far away and retreats to its assigned corner when close.
- Scatter mode: each ghost heads toward its assigned corner.
- Boss mode: the boss targets Pac-Man directly.

The game alternates between 20 seconds of chase mode and 7 seconds of scatter mode. The targeting algorithm is a simple local distance-based choice, not a full shortest-path search.

### 7.6 Boss mode and ghost collisions

After ghost movement, the game counts living ghosts. If one or fewer remain, any remaining living ghost is marked as a boss, given a higher speed, resized, and tinted red. A boss cannot be removed by the kill-ghost orb.

When a living ghost comes within 22 pixels of Pac-Man:

- If the kill-ghost power-up is active, and the ghost is not a boss, and more than one ghost is alive, the ghost is marked dead and 200 points are awarded (with the frenzy multiplier).
- Otherwise, if invincibility is not active, Pac-Man loses a life, returns to the starting position, and the ghosts are reset to their home tiles.

The game starts with one life, so losing that life leads to the game-over screen.

### 7.7 Other collectibles

**Grape:** The grape timer counts active run time. Every 300 seconds, a grape appears at pixel position (450, 306). Collecting it grants one additional life and removes the grape.

**Orange bonus orb:** The orange orb is located at the centre of maze tile row 8, column 11. It awards 1,000 points multiplied by the frenzy multiplier. It respawns 120 seconds after collection.

### 7.8 Gates and level progression

The entry gate is drawn at row 23, column 1. The exit gate is at row 1, column 23. The exit is displayed as locked until `got >= tot`.

`tot` is calculated once at the beginning of the run by counting all initial pellet tiles. `got` is a cumulative count of pellets collected during the current level. When `got` reaches `tot` and Pac-Man reaches the exit tile, the next level begins.

At level completion, the game:

1. Checks whether the level was completed in 120 seconds or less.
2. Updates the progress leaderboard with the number of levels cleared.
3. Increases the level speed multiplier by 10%.
4. Adds one active ghost, up to the maximum of eight.
5. Recreates the pellets and resets the player to the starting tile.
6. Resets power-up orbs and returns ghosts to their home positions.

There is no gameplay level cap in the supplied code. `LEVEL_TOTAL = 5` is used to cap the level count recorded by the progress leaderboard, not to stop the game from advancing.

### 7.9 Teleport passages

Pac-Man is teleported between the left and right sides when crossing the corresponding horizontal passage, and between the top and bottom when crossing the central vertical passage. The teleport checks are performed after the main movement update.

### 7.10 Pause and game over

Pressing P or Escape toggles the pause state. While paused, normal gameplay updates do not run. When lives reach zero, the current score and elapsed run time are saved once, the game-over overlay is displayed, and the top-five score list is shown. Pressing Space resets the run to level one.

## 8. Achievements and leaderboards

### Achievements

The game tracks five achievements:

| Achievement | Unlock condition |
|---|---|
| `SCORE 500` | Score reaches 500 |
| `SCORE 1500` | Score reaches 1,500 |
| `SCORE 3000` | Score reaches 3,000 |
| `GHOST HUNTER` | Pac-Man eliminates a ghost |
| `SPEED RUNNER` | A level is cleared in 120 seconds or less |

Achievement state is stored separately for each player in `ach_<PLAYER_NAME>.txt`. The entered name is limited to 10 letters or digits, which also keeps the generated filename simple.

### Top-three progress board

The left-side progress board ranks players using:

```text
progress total = levels cleared + achievements unlocked
```

Ties are broken by the number of achievements. Player progress is stored in `top_progress.txt`. The number of levels recorded is capped at five because `LEVEL_TOTAL` is five.

### Top-five score leaderboard

The game-over screen displays the five highest scores, alongside the run time in seconds. This separate leaderboard is stored in `leaderboard.txt`. Scores are sorted in descending order. The score leaderboard and the progress leaderboard are independent systems.

## 9. Audio system

| Audio | Behaviour |
|---|---|
| Menu music | Loops during the name-entry screen and stops when the game starts |
| Pellet sound | Plays at 50% volume when a pellet is collected, if no tracked higher-priority audio is playing |
| Lane-warning playlist | Plays one of four clips in sequence when a living ghost enters Pac-Man's unobstructed row or column |
| Death playlist | Plays one of three clips in sequence when Pac-Man loses a life |

The lane-warning test requires the ghost and Pac-Man to be aligned within a three-pixel tolerance and have no wall tile between them. The warning is triggered on entry into that clear lane, rather than being restarted every frame. A lane warning interrupts the pellet sound. Death handling stops the pellet and lane-warning sounds before playing the next death clip.

Audio files are loaded from the program's working directory. If a file cannot be loaded, the program prints a warning to the console.

## 10. Important functions

| Function | Responsibility |
|---|---|
| `place_dots()` | Places pellets on eligible empty maze tiles |
| `isWall()` | Checks whether a pixel coordinate is inside a normal wall tile or outside the maze |
| `isWall2()` | Checks whether a maze row/column contains a wall tile |
| `canMoveTo()` | Tests the four corners of Pac-Man's collision box |
| `ghostSharesClearLane()` | Tests whether a ghost shares a clear horizontal or vertical lane with Pac-Man |
| `orbsUpdate()` | Updates power-up effect timers and orb respawn timers |
| `orbsPickup()` | Detects collected power-up orbs, starts effects, and returns bonus points |
| `orbsReset()` | Clears power-up timers and optionally restores all orbs |
| `orbsDraw()` / `orbsDrawHud()` | Draws active orbs and their remaining effect timers |
| `achLoad()` / `achSave()` | Loads and saves a player's achievements |
| `achUnlock()` | Unlocks an achievement, displays a popup, and updates progress ranking |
| `achCheckScore()` | Checks score-based achievement thresholds |
| `achGhostEaten()` | Unlocks the ghost-hunter achievement |
| `achLevelCleared()` | Checks the level completion-time achievement |
| `achDraw()` | Draws the achievement panel |
| `lbLoad()` / `lbSave()` | Loads and saves the progress leaderboard |
| `lbUpdate()` | Adds or updates a player's level and achievement totals |
| `lbSort()` | Sorts players by total progress and achievement count |
| `lbDraw()` | Draws the top-three progress panel |
| `loadScores()` / `addScore()` | Loads and updates the top-five score leaderboard |
| `drawLeaderboard()` | Draws the top-five score list on the game-over screen |
| `drawWallLayer()` | Draws connected wall shapes used for the glowing maze-wall effect |
| `main()` | Creates game resources, handles input and game updates, and renders each frame |

## 11. Required assets and generated data files

All listed resource files are opened by filename, so they should be present in the program's working directory unless the paths in the code are changed.

### Graphics and font assets

| Filename | Purpose |
|---|---|
| `maze.bmp` | Maze background image |
| `Player1_TextureA.png` | First Pac-Man animation frame |
| `Player1_TextureB.png` | Second Pac-Man animation frame |
| `red_ghost.png` | Red ghost texture |
| `pink_ghost.png` | Pink ghost texture |
| `bluee.png` | Blue/scared ghost texture |
| `yellow_ghost.png` | Yellow ghost texture |
| `Game_banner.jpg` | Left and right panel background |
| `grape.png` | Extra-life collectible image |
| `pixel_font.otf` | Game text font |

### Audio assets

| Filename | Purpose |
|---|---|
| `Pacman+-+Eating+(Sound+Effect).mp3` | Pellet collection sound |
| `breakzstudios-electronic-uplifting-retro-game-hidden-gem-short-version-210310.mp3` | Name-entry/menu music |
| `pakad-mc.mp3` | First lane-warning clip |
| `aree-cid.mp3` | Second lane-warning clip |
| `cid-maiya-chood.mp3` | Third lane-warning clip |
| `cid-aak-bar-mil-jabeta.mp3` | Fourth lane-warning clip |
| `ghost3.mp3.mpeg` | First death clip |
| `cid-acp-pradhuman-fasi.mp3` | Second death clip |
| `cid-ke-bakchodi.mp3` | Third death clip |

### Files created or updated by the game

| Filename pattern | Purpose |
|---|---|
| `ach_<PLAYER_NAME>.txt` | Achievement flags for an individual player |
| `top_progress.txt` | Top-three progress leaderboard data source |
| `leaderboard.txt` | Top-five score and run-time records |

## 12. Build requirements

The code uses SFML 3-style APIs, including `std::optional` event polling and the SFML 3 event/key interfaces. It should therefore be built against **SFML 3**, not SFML 2.x. SFML 3 requires a C++17-capable compiler. The official SFML FAQ lists GCC 9, Clang 9, and MSVC 2019 (MSVC 16) as minimum compiler versions. See the official [SFML build and use FAQ](https://www.sfml-dev.org/faq/build-use/).

The program uses the SFML Graphics, Window, Audio, and System modules. With GCC/MinGW and correctly configured SFML library paths, a typical link command is:

```bash
g++ -std=c++17 main.cpp -o PacMan \
    -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
```

This command assumes the compiler can already find the SFML headers and libraries. Configure the include and library directories for your own SFML installation. For dynamic builds, ensure the required SFML runtime DLLs are available when launching the executable. The official SFML CMake tutorial recommends using the SFML CMake project template for new projects: https://www.sfml-dev.org/tutorials/3.0/getting-started/cmake/.

### Running the game

1. Build the source using SFML 3 and C++17.
2. Place all required image, font, and audio assets in the program's working directory.
3. Run the generated executable from that directory so the relative asset paths resolve correctly.
4. Enter a player name using letters and digits, then press Enter.

## 13. Key configuration values

| Constant or variable | Current value | Meaning |
|---|---:|---|
| `CELL` | 36 | Maze tile size in pixels |
| `COLS`, `ROWS` | 25, 25 | Maze dimensions |
| `ORB_COUNT` | 4 | Number of power-up orbs |
| `ORB_RESPAWN_TIME` | 60 seconds | Power-up orb respawn delay |
| `SPEED_TIME` | 10 seconds | Speed effect duration |
| `INVINCIBLE_TIME` | 10 seconds | Invincibility duration |
| `KILL_TIME` | 10 seconds | Ghost-killing effect duration |
| `SPEED_BOOST` | 1.6 | Speed-orb multiplier |
| `RECORD_TIME` | 120 seconds | Maximum time for the speed-runner achievement |
| `LEVEL_TOTAL` | 5 | Maximum level count recorded by the progress board |
| `MAX_GHOSTS` | 8 | Maximum active ghosts |
| `rsT` | 300 seconds | Pellet respawn delay |
| `gsp` | 300 seconds | Grape spawn interval |
| `bsp` | 120 seconds | Bonus-orb respawn delay |
| `Player::speed` | 120 px/s | Pac-Man base speed |
| `Ghost::speed` | 130 px/s | Normal ghost base speed |

## 14. Implementation notes and current limitations

- `Player::current_state` and `Ghost::state` are declared but are not used by the supplied gameplay logic.
- `achDrawOld()` and `lbDrawOld()` are legacy drawing functions; the active rendering code calls `achDraw()` and `lbDraw()` instead.
- `got` records the cumulative number of pellets collected in the current level. It is not the number of pellets currently absent from the maze. Pellet respawning does not reduce `got`.
- `tot` is calculated at the start of the program. The supplied maze layout is reused at each level, so the same initial target is used for subsequent levels.
- `LEVEL_TOTAL` limits the progress recorded by the progress leaderboard to five levels. It does not end the actual game at level five.
- Ghost movement uses local distance comparisons and is not a full pathfinding algorithm. It may make imperfect choices around complex maze layouts.
- Ghosts reject `-1` tiles during path selection, while Pac-Man's normal wall check only treats tile value `1` as a wall. The edge teleport logic is handled separately for Pac-Man.
- The score leaderboard and progress leaderboard use different files and ranking rules.
- The code uses relative filenames. Running the executable from a different working directory can cause resource-loading failures even if the files exist elsewhere.
- The score display's manual number-to-string conversion only handles non-negative values correctly. Since the score begins at -10, the HUD may not display that initial negative value correctly before the starting pellet is collected.
- The code prints audio-loading errors if a file cannot be opened. Confirm that the installed SFML audio build supports the chosen audio formats if an audio file fails to load.

## 15. Overall program flow

1. Create the game window, view, textures, sprites, fonts, and interface elements.
2. Initialize player, ghost, maze, collectibles, achievement, leaderboard, and audio state.
3. Display the name-entry screen and play menu music.
4. Load the selected player's achievements and update the progress leaderboard.
5. Enter the main game loop:
   - Process window and pause events.
   - Calculate frame time.
   - Update frenzy, power-ups, achievements, player movement, pellets, gates, ghosts, collisions, and collectibles.
   - Update score and lives displays.
   - Draw the maze, effects, interface, player, ghosts, and level banner.
6. When lives reach zero, save the run's score, display the game-over screen, and wait for Space to restart.
7. On restart, reset the run, return to level one, restore ghosts and collectibles, and continue the main loop.

---

*This document describes the behaviour visible in the supplied source code. It does not imply that the project has been compiled or tested with the listed assets and SFML installation.*

