# Pac-Man Game Proposal

**Team Lead:** Rayan Khan (26L-2633)

**Team Members:**
- Muhammad Abdullah (26L-2554)
- Muhammad Hamza (26L-2556)
- Khizar Rehman (26L-2574)

**Date:** October 18, 2026

---

## 1. Introduction

The aim of this project is to build a fully functioning pac man clone with 10 additional unique features which set it apart from the traditional pac man game.

## 2. Objectives

1. Apply the concepts of programming to get to know about practical applications of C++ concepts
2. Build a fully functioning game, which is playable outside the scope of the project
3. Use the SFML graphics library to render the maze, characters, and animations in real time.
4. Provide smooth, responsive controls and a clear interface showing the score, lives, and game state.
5. Use and learn GitHub for seamless project collaboration

## 3. Libraries and Concepts Used

The project will be made using SFML 3.0, iostream will also be utilized where necessary. Pf concepts like arrays will be used to keep track of maze data. SFML features like event tracking will be heavily used while some for some features, the audio features will be utilized.

## 4. Features and Functionality

### 4.1 Multiple Power-Up Orbs

In addition to the standard power-up orb, the maze has several types of orb. A speed orb makes Pac-Man move faster, an invincibility orb protects him from ghosts for a short time, and a kill-ghost orb lets him eliminate ghosts.

### 4.2 Special Bonus Orb

A special point orb sits in front of the monster home (the ghost spawn area). Collecting it gives 500 bonus points, which rewards players who take the risk of going near ghosts.

### 4.3 Extra Teleportation Points

Two more teleportation points are added, one above and one below the maze. Together with the existing side tunnels, they give the player more ways to escape ghosts and move around quickly.

### 4.4 Personal Leaderboard

A personal leader-board saves and displays the player's top 5 scores along with the time of each run. Scores are stored in a file, so they persist between sessions.

### 4.5 Frenzy Mode

Once every minute, a 10-second frenzy mode starts. During it, ghosts spawn at double the rate and all points earned are multiplied by 5, creating a short high-risk, high-reward phase.

### 4.6 Gates on Lower Left and Upper Right Corners

The player enters the stage through the lower left corner, then they proceed to play the stage with the objective of collecting all available points, once all points are collected the player must reach the upper right corner and exit through it to clear the level. It is important to note that one can only exit upper right gate once they collect the number of points that were on that stage at the start (e.g. if at the start there were 50 pellets on the stage, then one can exit only after collecting 50 pellets but respawning pellets encourage continued play even after exit condition reached).

### 4.7 Ghost Voice Taunts

Ghosts play voice taunts that make fun of the player, for example, when he loses a life or fails to escape. This adds humor and personality to the game-play.

### 4.8 Special Boss Ghost

If all of the current ghosts die except 1, then that ghost enters a boss stage where all of its attributes are enhanced (like speed), when in a boss state the ghost cannot die. This means that throughout the game, 1 ghost will always stay alive never making the game boring.

### 4.9 User Achievements

The game tracks the player's progress and unlocks achievements for milestones particularly clearing the level in record times, players can also get achievements for having a large number of points which encourages then to go for extra points.

### 4.10 Extra Pellets for Points

After each frenzy mode is over, all of the pellets on the map respawn, this enocurages continued play and reaching for point records.

### 4.11 Grapee Idea

In every game, unlike the traditional pac man game which has 3 lives, this game gives the player only 1 life, but there is a grape in the middle of the stage right outside the monster spawner, if the player eats this fruit then they gain another extra life. This Grape spawns every 5 minutes and ensures they players are rewarded for surviving for a long time and taking a bit of risk, this also gives then a cushion for longer runs.
