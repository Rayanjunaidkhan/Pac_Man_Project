#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <optional> // Abd
#include <iostream>
#include <cmath>   // Done by MUHAMMAD KHIZAR REHMAN
#include <cstdlib> // rand / srand (random ghost decisions, random orb spawning)
#include <ctime>   // time stamps on leaderboard runs, seed for rand

const int PLAYER_NAME_CAPACITY = 11;
const int PLAYER_NAME_LIMIT = 10;

struct Player
{
    sf::Sprite sprite;
    int lives = 1;   // Done by MUHAMMAD KHIZAR REHMAN: only 1 life as in the proposal
    int score = -10;
    int current_state = 0; // 0 for normal, 1 for eating ghost mode, 2 for faster speed
    int speed = 120;

    Player(const sf::Texture& tex) : sprite(tex) {}
};

struct Ghost
{
    sf::Sprite sprite;
    int state = 0; // 0  is normal, 1 is scared
    int speed = 130;
    // abd work
    bool alive = true;      // NEW
    bool isBoss = false;    // NEW
    Ghost(const sf::Texture& tex) : sprite(tex) {}
};

// these are function prototypes

// Creates the pellets that Pac-Man can collect in the maze.
void placeMazeDots();
// Draws one glowing layer over the maze walls.
void drawMazeWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states);

// making constants coz these are fixed mapping values of game.
const int TILE_SIZE = 36;   // size of one square, in pixels
const int MAZE_COLUMNS = 25;   // how many squares across
const int MAZE_ROWS = 25;   // how many squares down


// 0 for path with point, 1 for wall, -1 for path without point
int mazeTiles[MAZE_COLUMNS * MAZE_ROWS] = {
1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,1,1,1,1,1,1,1,1,1,1, // row 0
1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1, // row 1
1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,1, // row 2
1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1, // row 3
1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1, // row 4
1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1, // row 5
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1, // row 6
1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1, // row 7
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1, // row 8
1,0,0,0,0,0,0,1,0,0,1,1,0,1,1,0,0,1,0,0,0,0,0,0,1, // row 9
1,1,1,1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1, // row 10
-1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,-1, // row 11
1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1, // row 12
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1, // row 13
1,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,1, // row 14
1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1, // row 15
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1, // row 16
1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1, // row 17
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1, // row 18
1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1, // row 19
1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1, // row 20
1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1, // row 21
1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,1, // row 22
1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1, // row 23
1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,1,1,1,1,1,1,1,1,1,1  // row 24
};

// This is abdullah ameer code for making orbits and speeding process
// ===== ORB CODE (5.1 Multiple Power-Up Orbs) =====
const int   POWER_UP_COUNT = 4;
const int   POWER_UP_TILE_SIZE = 36;
const int  powerUpStartRows[POWER_UP_COUNT] = { 1, 1, 13, 23 };   // where each orb starts every level
const int  powerUpStartColumns[POWER_UP_COUNT] = { 1, 23, 12, 23 };
int  powerUpRows[POWER_UP_COUNT] = { 1, 1, 13, 23 };
int  powerUpColumns[POWER_UP_COUNT] = { 1, 23, 12, 23 };
int  powerUpTypes[POWER_UP_COUNT] = { 1, 2, 3, 1 };    // 1 = speed, 2 = invincible, 3 = kill-ghost
bool powerUpIsVisible[POWER_UP_COUNT] = { true, true, true, true };
const float POWER_UP_RESPAWN_SECONDS = 60.f;
float powerUpRespawnTimers[POWER_UP_COUNT] = { 0.f, 0.f, 0.f, 0.f };

const float SPEED_POWER_SECONDS = 10.f;
const float SHIELD_POWER_SECONDS = 10.f;
const float GHOST_KILL_POWER_SECONDS = 10.f;
const float SPEED_POWER_MULTIPLIER = 1.6f;

float speedPowerTimer = 0, shieldPowerTimer = 0, ghostKillPowerTimer = 0;
// working for next levels by abd
int   currentLevel = 1;               // abhi kaun sa level hai
float levelSpeedMultiplier = 1.f;        // har level par x1.10
int   activeGhostCount = 4;          // abhi kitne ghosts khel mein hain
const int MAX_GHOST_COUNT = 8;
float levelBannerTimer = 0;         // "LEVEL 2" screen ka timer
// Chooses the display color for a power-up based on its type.
sf::Color getPowerUpColor(int powerUpType)
{
    if (powerUpType == 1)
        return sf::Color(0, 220, 255);
    if (powerUpType == 2)
        return sf::Color(255, 215, 0);
    return sf::Color(255, 40, 40);
}

// Moves a power-up to a random open tile that is not already in use.
void placePowerUpOnRandomTile(int powerUpIndex)
{
    for (int randomTileAttempts = 0; randomTileAttempts < 200; randomTileAttempts++)
    {
        int randomRow = std::rand() % MAZE_ROWS;
        int randomColumn = std::rand() % MAZE_COLUMNS;

        if (mazeTiles[randomRow * MAZE_COLUMNS + randomColumn] != 0 &&
            mazeTiles[randomRow * MAZE_COLUMNS + randomColumn] != 2)
            continue;                                   // wall or tunnel edge
        if (randomRow >= 9 && randomRow <= 12 && randomColumn >= 10 && randomColumn <= 14)
            continue;                                   // ghost house
        if (randomRow == 8 && (randomColumn == 11 || randomColumn == 12))
            continue;                                   // bonus orb + grape tiles
        if ((randomRow == 23 && randomColumn == 1) ||
            (randomRow == 1 && randomColumn == 23))
            continue;                                   // entry / exit gate

        bool tileIsTaken = false;
        for (int otherPowerUpIndex = 0; otherPowerUpIndex < POWER_UP_COUNT; otherPowerUpIndex++)
            if (otherPowerUpIndex != powerUpIndex &&
                powerUpIsVisible[otherPowerUpIndex] &&
                powerUpRows[otherPowerUpIndex] == randomRow &&
                powerUpColumns[otherPowerUpIndex] == randomColumn)
                tileIsTaken = true;
        if (tileIsTaken)
            continue;

        powerUpRows[powerUpIndex] = randomRow;
        powerUpColumns[powerUpIndex] = randomColumn;
        return;
    }
}

// Counts down active power-ups and brings collected orbs back later.
void updatePowerUps(float deltaTime)
{
    if (speedPowerTimer > 0)
    {
        speedPowerTimer -= deltaTime;
        if (speedPowerTimer < 0.f)
            speedPowerTimer = 0.f;
    }
    if (shieldPowerTimer > 0)
    {
        shieldPowerTimer -= deltaTime;
        if (shieldPowerTimer < 0.f)
            shieldPowerTimer = 0.f;
    }
    if (ghostKillPowerTimer > 0)
    {
        ghostKillPowerTimer -= deltaTime;
        if (ghostKillPowerTimer < 0.f)
            ghostKillPowerTimer = 0.f;
    }

    // Each collected power-up orb respawns 60 seconds after it was collected,
    // on a random tile.
    for (int powerUpIndex = 0; powerUpIndex < POWER_UP_COUNT; powerUpIndex++)
    {
        if (!powerUpIsVisible[powerUpIndex] && powerUpRespawnTimers[powerUpIndex] > 0.f)
        {
            powerUpRespawnTimers[powerUpIndex] -= deltaTime;
            if (powerUpRespawnTimers[powerUpIndex] <= 0.f)
            {
                powerUpRespawnTimers[powerUpIndex] = 0.f;
                placePowerUpOnRandomTile(powerUpIndex);
                powerUpIsVisible[powerUpIndex] = true;
            }
        }
    }
}
// Returns the player's speed multiplier, including the current level.
float getPlayerSpeedMultiplier()
{
    float powerUpMultiplier = 1.f;
    if (speedPowerTimer > 0)
        powerUpMultiplier = SPEED_POWER_MULTIPLIER;
    return powerUpMultiplier * levelSpeedMultiplier;
}

// Tells whether the shield power-up is still active.
bool  isPlayerShieldActive()
{
    return shieldPowerTimer > 0;
}
// Tells whether the power-up that can defeat ghosts is active.
bool  isGhostKillPowerActive()
{
    return ghostKillPowerTimer > 0;
}


// Collects nearby power-ups, starts their timers, and returns bonus points.
int collectPowerUps(sf::Vector2f playerPosition)
{
    int bonusPoints = 0;
    for (int powerUpIndex = 0; powerUpIndex < POWER_UP_COUNT; powerUpIndex++)
    {
        if (!powerUpIsVisible[powerUpIndex]) continue;
        float distanceX = playerPosition.x - (powerUpColumns[powerUpIndex] + 0.5f) * POWER_UP_TILE_SIZE;
        float distanceY = playerPosition.y - (powerUpRows[powerUpIndex] + 0.5f) * POWER_UP_TILE_SIZE;
        if (distanceX * distanceX + distanceY * distanceY < 18.f * 18.f)
        {
            powerUpIsVisible[powerUpIndex] = false;
            powerUpRespawnTimers[powerUpIndex] = POWER_UP_RESPAWN_SECONDS;
            if (powerUpTypes[powerUpIndex] == 1) speedPowerTimer = SPEED_POWER_SECONDS;
            else if (powerUpTypes[powerUpIndex] == 2) shieldPowerTimer = SHIELD_POWER_SECONDS;
            else ghostKillPowerTimer = GHOST_KILL_POWER_SECONDS;
            bonusPoints += 50;
        }
    }
    return bonusPoints;
}

// Clears power-up timers and optionally returns every orb to its start tile.
void resetPowerUps(bool respawnOrbs)
{
    speedPowerTimer = 0;
    shieldPowerTimer = 0;
    ghostKillPowerTimer = 0;
    if (respawnOrbs)
    {
        for (int powerUpIndex = 0; powerUpIndex < POWER_UP_COUNT; powerUpIndex++)
        {
            powerUpIsVisible[powerUpIndex] = true;
            powerUpRespawnTimers[powerUpIndex] = 0.f;
            powerUpRows[powerUpIndex] = powerUpStartRows[powerUpIndex];      // back to the starting tile
            powerUpColumns[powerUpIndex] = powerUpStartColumns[powerUpIndex];
        }
    }
}


// Draws each power-up orb that is currently visible.
void drawPowerUps(sf::RenderWindow& window, float elapsedTime)
{
    sf::CircleShape shape;
    for (int powerUpIndex = 0; powerUpIndex < POWER_UP_COUNT; powerUpIndex++)
    {
        if (!powerUpIsVisible[powerUpIndex]) continue;
        float orbRadius = 9.f + 2.f * std::sin(elapsedTime * 6.f);
        shape.setRadius(orbRadius);
        shape.setOrigin({ orbRadius, orbRadius });
        shape.setPosition({ (powerUpColumns[powerUpIndex] + 0.5f) * POWER_UP_TILE_SIZE,
                            (powerUpRows[powerUpIndex] + 0.5f) * POWER_UP_TILE_SIZE });
        shape.setFillColor(getPowerUpColor(powerUpTypes[powerUpIndex]));
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
    }
}

// Shows the remaining time for each active power-up on the screen.
void drawPowerUpTimers(sf::RenderWindow& window, const sf::Font& font)
{
    char text[96] = "";
    if (speedPowerTimer > 0)
        std::snprintf(text + std::strlen(text), sizeof(text) - std::strlen(text),
            "SPEED: %d\n", int(speedPowerTimer) + 1);
    if (shieldPowerTimer > 0)
        std::snprintf(text + std::strlen(text), sizeof(text) - std::strlen(text),
            "SHIELD: %d\n", int(shieldPowerTimer) + 1);
    if (ghostKillPowerTimer > 0)
        std::snprintf(text + std::strlen(text), sizeof(text) - std::strlen(text),
            "KILL: %d\n", int(ghostKillPowerTimer) + 1);
    if (text[0] == '\0') return;
    sf::Text powerUpText(font, text, 30);
    powerUpText.setFillColor(sf::Color::Red);
    powerUpText.setPosition({ 930.f, 430.f });
    window.draw(powerUpText);
}
// ===== END OF ORB CODE =====
// 
// 
// 
// 
// abdillahameer added achievement board
// ===== ACHIEVEMENTS + TOP 3 PLAYERS (Abdullah Ameer) =====
const int   ACHIEVEMENT_COUNT = 5;
const float FAST_LEVEL_TIME_LIMIT = 120.f;
const float ACHIEVEMENT_PANEL_X = -280.f;
const float ACHIEVEMENT_PANEL_Y = 330.f;
const int   TOTAL_LEVEL_COUNT = 5;
const float LEADERBOARD_PANEL_X = -280.f;
const float LEADERBOARD_PANEL_Y = 60.f;
const int   MAX_LEADERBOARD_ENTRIES = 50;

const char achievementNames[ACHIEVEMENT_COUNT][32] = {
    "SCORE 500", "SCORE 1500", "SCORE 3000", "GHOST HUNTER", "SPEED RUNNER"
};
bool  achievementUnlocked[ACHIEVEMENT_COUNT] = { false, false, false, false, false };
float achievementPopupTimer = 0;
char achievementPopupText[64] = "";
char achievementPlayerName[PLAYER_NAME_CAPACITY] = "PLAYER";
float levelStartTime = 0;
// Abdullah Ameer added the achievements board.
// Declares the leaderboard update function used when achievements change.
void updateProgressLeaderboard(const char name[], int count);   // forward declaration

// Counts how many achievements the current player has unlocked.
int countUnlockedAchievements()
{
    int unlockedCount = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        if (achievementUnlocked[i]) unlockedCount++;
    return unlockedCount;
}
// Saves the current player's achievement progress to a file.
void saveAchievements()
{
    char filename[PLAYER_NAME_CAPACITY + 8];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", achievementPlayerName);
    std::ofstream f(filename);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        int unlockedValue = 0;
        if (achievementUnlocked[i])
            unlockedValue = 1;
        f << unlockedValue << " ";
    }
}

// Loads a player's saved achievements and clears missing ones.
void loadPlayerAchievements(const char name[])
{
    std::snprintf(achievementPlayerName, sizeof(achievementPlayerName), "%s", name);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        achievementUnlocked[i] = false;
    char filename[PLAYER_NAME_CAPACITY + 8];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", achievementPlayerName);
    std::ifstream f(filename);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        int savedValue = 0;
        if (f >> savedValue) achievementUnlocked[i] = (savedValue == 1);
    }
}

// Unlocks an achievement once, shows a message, and saves the result.
void unlockAchievement(int achievementIndex)
{
    if (achievementUnlocked[achievementIndex]) return;
    achievementUnlocked[achievementIndex] = true;
    std::snprintf(achievementPopupText, sizeof(achievementPopupText),
        "UNLOCKED: %s", achievementNames[achievementIndex]);
    achievementPopupTimer = 3.f;
    saveAchievements();
    updateProgressLeaderboard(achievementPlayerName, 0);
}

// Unlocks score achievements when the player reaches each target.
void checkScoreAchievements(int score)
{
    if (score >= 500)  unlockAchievement(0);
    if (score >= 1500) unlockAchievement(1);
    if (score >= 3000) unlockAchievement(2);
}

// Unlocks the achievement for eating a ghost.
void unlockGhostAchievement() { unlockAchievement(3); }

// Unlocks the speed-run achievement when a level is cleared in time.
void checkFastLevelAchievement(float seconds)
{
    if (seconds <= FAST_LEVEL_TIME_LIMIT) unlockAchievement(4);
}

// Decreases the time left for the achievement message.
void updateAchievementPopup(float deltaTime)
{
    if (achievementPopupTimer > 0) achievementPopupTimer -= deltaTime;
}

// Draws the older achievement list and any unlock message.
void drawOldAchievementBoard(sf::RenderWindow& window, const sf::Font& font)
{
    char playerTitle[64];
    std::snprintf(playerTitle, sizeof(playerTitle), "PLAYER: %s", achievementPlayerName);
    sf::Text who(font, playerTitle, 22);
    who.setFillColor(sf::Color::White);
    who.setPosition({ ACHIEVEMENT_PANEL_X, ACHIEVEMENT_PANEL_Y });
    window.draw(who);

    sf::Text title(font, "ACHIEVEMENTS", 24);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({ ACHIEVEMENT_PANEL_X, ACHIEVEMENT_PANEL_Y + 26.f });
    window.draw(title);

    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        char line[64];
        char mark[5] = "[ ] ";
        if (achievementUnlocked[i])
            std::snprintf(mark, sizeof(mark), "[X] ");
        std::snprintf(line, sizeof(line), "%s%s", mark, achievementNames[i]);
        sf::Text t(font, line, 20);
        if (achievementUnlocked[i])
            t.setFillColor(sf::Color(255, 215, 0));
        else
            t.setFillColor(sf::Color(160, 160, 160));
        t.setPosition({ ACHIEVEMENT_PANEL_X, ACHIEVEMENT_PANEL_Y + 58.f + i * 22.f });
        window.draw(t);
    }

    if (achievementPopupTimer > 0)
    {
        sf::Text pop(font, achievementPopupText, 36);
        pop.setFillColor(sf::Color::Yellow);
        pop.setOutlineThickness(3.f);
        pop.setOutlineColor(sf::Color::Black);
        pop.setPosition({ 200.f, 20.f });
        window.draw(pop);
    }
}
char leaderboardPlayerNames[MAX_LEADERBOARD_ENTRIES][PLAYER_NAME_CAPACITY] = {};
int leaderboardLevelsCleared[MAX_LEADERBOARD_ENTRIES];   // levels cleared (0..LEVEL_TOTAL)
int leaderboardAchievementsUnlocked[MAX_LEADERBOARD_ENTRIES];      // achievements unlocked (0..ACH_COUNT)
int leaderboardEntryCount = 0;

// Adds a player's cleared levels and achievements for ranking.
int getLeaderboardPlayerTotal(int i) { return leaderboardLevelsCleared[i] + leaderboardAchievementsUnlocked[i]; }

// Counts the achievements saved in the named player's file.
int countPlayerAchievements(const char name[])
{
    char filename[PLAYER_NAME_CAPACITY + 8];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", name);
    std::ifstream a(filename);
    int unlockedCount = 0;
    int savedValue = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT && (a >> savedValue); i++)
        if (savedValue == 1) unlockedCount++;
    return unlockedCount;
}

// Saves the progress leaderboard to a file.
void saveProgressLeaderboard()
{
    std::ofstream f("top_progress.txt");
    for (int i = 0; i < leaderboardEntryCount; i++)
        f << leaderboardPlayerNames[i] << " " << leaderboardLevelsCleared[i] << "\n";
}

// Sorts the leaderboard from highest progress to lowest.
void sortProgressLeaderboard()
{
    for (int i = 0; i < leaderboardEntryCount - 1; i++)
    {
        for (int j = 0; j < leaderboardEntryCount - 1 - i; j++)
        {
            bool shouldSwapEntries = getLeaderboardPlayerTotal(j) < getLeaderboardPlayerTotal(j + 1) ||
                (getLeaderboardPlayerTotal(j) == getLeaderboardPlayerTotal(j + 1) && leaderboardAchievementsUnlocked[j] < leaderboardAchievementsUnlocked[j + 1]);
            if (shouldSwapEntries)
            {
                char name[PLAYER_NAME_CAPACITY];
                std::snprintf(name, sizeof(name), "%s", leaderboardPlayerNames[j]);
                std::snprintf(leaderboardPlayerNames[j], sizeof(leaderboardPlayerNames[j]), "%s", leaderboardPlayerNames[j + 1]);
                std::snprintf(leaderboardPlayerNames[j + 1], sizeof(leaderboardPlayerNames[j + 1]), "%s", name);

                int t = leaderboardLevelsCleared[j];
                leaderboardLevelsCleared[j] = leaderboardLevelsCleared[j + 1];
                leaderboardLevelsCleared[j + 1] = t;

                t = leaderboardAchievementsUnlocked[j];
                leaderboardAchievementsUnlocked[j] = leaderboardAchievementsUnlocked[j + 1];
                leaderboardAchievementsUnlocked[j + 1] = t;
            }
        }
    }
}

// Loads saved leaderboard entries and sorts them.
void loadProgressLeaderboard()
{
    leaderboardEntryCount = 0;
    std::ifstream f("top_progress.txt");
    while (leaderboardEntryCount < MAX_LEADERBOARD_ENTRIES)
    {
        f.width(PLAYER_NAME_CAPACITY);
        if (!(f >> leaderboardPlayerNames[leaderboardEntryCount] >> leaderboardLevelsCleared[leaderboardEntryCount]))
            break;
        leaderboardEntryCount++;
    }

    for (int i = 0; i < leaderboardEntryCount; i++)
    {
        if (leaderboardLevelsCleared[i] > TOTAL_LEVEL_COUNT) leaderboardLevelsCleared[i] = TOTAL_LEVEL_COUNT;
        leaderboardAchievementsUnlocked[i] = countPlayerAchievements(leaderboardPlayerNames[i]);
    }
    sortProgressLeaderboard();
}

// Adds or updates a player's level progress and saves the ranking.
void updateProgressLeaderboard(const char name[], int levels)
{
    if (levels > TOTAL_LEVEL_COUNT)
        levels = TOTAL_LEVEL_COUNT;

    int playerIndex = -1;
    for (int i = 0; i < leaderboardEntryCount; i++)
        if (std::strcmp(leaderboardPlayerNames[i], name) == 0) { playerIndex = i; break; }

    if (playerIndex == -1)
    {
        if (leaderboardEntryCount >= MAX_LEADERBOARD_ENTRIES) return;
        playerIndex = leaderboardEntryCount++;
        std::snprintf(leaderboardPlayerNames[playerIndex], sizeof(leaderboardPlayerNames[playerIndex]), "%s", name);
        leaderboardLevelsCleared[playerIndex] = 0;
    }
    if (levels > leaderboardLevelsCleared[playerIndex])
        leaderboardLevelsCleared[playerIndex] = levels;
    leaderboardAchievementsUnlocked[playerIndex] = countPlayerAchievements(name);

    sortProgressLeaderboard();
    saveProgressLeaderboard();
}

// Draws the older compact progress leaderboard.
void drawOldProgressLeaderboard(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text title(font, "TOP 3 PLAYERS", 22);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({ LEADERBOARD_PANEL_X, LEADERBOARD_PANEL_Y });
    window.draw(title);

    for (int i = 0; i < 3 && i < leaderboardEntryCount; i++)
    {
        char line[64];
        std::snprintf(line, sizeof(line), "%d. %s  %d/%d",
            i + 1, leaderboardPlayerNames[i], leaderboardLevelsCleared[i], TOTAL_LEVEL_COUNT);
        sf::Text t(font, line, 20);
        if (i == 0)
            t.setFillColor(sf::Color(255, 215, 0));
        else
            t.setFillColor(sf::Color::White);
        t.setPosition({ LEADERBOARD_PANEL_X, LEADERBOARD_PANEL_Y + 26.f + i * 24.f });
        window.draw(t);
    }
}




// ===== PRETTY BOARDS (presentable look) =====
// Draws left-aligned text at the given screen position.
void drawPanelText(sf::RenderWindow& w, const sf::Font& font, const char s[], unsigned int size, sf::Color col, float x, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    t.setPosition({ x, y });
    w.draw(t);
}

// Draws text centered around the given horizontal position.
void drawCenteredPanelText(sf::RenderWindow& w, const sf::Font& font, const char s[], unsigned int size, sf::Color col, float cx, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, 0.f });
    t.setPosition({ cx, y });
    w.draw(t);
}

// Draws a dark panel with a gold outline.
void drawPanelBox(sf::RenderWindow& w, float x, float y, float width, float height)
{
    sf::RectangleShape r({ width, height });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(0, 0, 0, 190));
    r.setOutlineThickness(2.f);
    r.setOutlineColor(sf::Color(255, 215, 0));
    w.draw(r);
}

// Draws a thin gold divider inside a panel.
void drawPanelLine(sf::RenderWindow& w, float x, float y, float width)
{
    sf::RectangleShape r({ width, 2.f });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(255, 215, 0, 160));
    w.draw(r);
}


// Draws the three highest-ranked players and their progress.
void drawProgressLeaderboard(sf::RenderWindow& window, const sf::Font& font)
{
    const float panelX = -290.f, panelWidth = 280.f, top = 40.f;
    const int MAX_PROGRESS_POINTS = TOTAL_LEVEL_COUNT + ACHIEVEMENT_COUNT;
    sf::Color medal[3] = { sf::Color(255, 215, 0), sf::Color(200, 200, 215), sf::Color(205, 127, 50) };

    drawPanelBox(window, panelX, top, panelWidth, 200.f);
    drawCenteredPanelText(window, font, "TOP 3 PLAYERS", 26, sf::Color::Yellow, panelX + panelWidth / 2.f, top + 8.f);
    drawPanelLine(window, panelX + 10.f, top + 44.f, panelWidth - 20.f);

    for (int i = 0; i < 3; i++)
    {
        float rowY = top + 56.f + i * 46.f;
        bool hasPlayer = (i < leaderboardEntryCount);

        sf::CircleShape badge(13.f);
        badge.setPosition({ panelX + 12.f, rowY + 4.f });
        if (hasPlayer)
            badge.setFillColor(medal[i]);
        else
            badge.setFillColor(sf::Color(70, 70, 70));
        window.draw(badge);
        char rankText[8];
        std::snprintf(rankText, sizeof(rankText), "%d", i + 1);
        drawCenteredPanelText(window, font, rankText, 18, sf::Color::Black, panelX + 25.f, rowY + 6.f);

        if (!hasPlayer)
        {
            drawPanelText(window, font, "---", 22, sf::Color(120, 120, 120), panelX + 48.f, rowY + 4.f);
            continue;
        }

        sf::Color playerNameColor = sf::Color::White;
        if (i == 0)
            playerNameColor = sf::Color(255, 215, 0);
        drawPanelText(window, font, leaderboardPlayerNames[i], 22, playerNameColor, panelX + 48.f, rowY);

        for (int s = 0; s < MAX_PROGRESS_POINTS; s++)
        {
            sf::RectangleShape seg({ 13.f, 8.f });
            seg.setPosition({ panelX + 48.f + s * 16.f, rowY + 30.f });
            if (s < getLeaderboardPlayerTotal(i))
                seg.setFillColor(medal[i]);
            else
                seg.setFillColor(sf::Color(70, 70, 70));
            window.draw(seg);
        }

        char progressText[16];
        std::snprintf(progressText, sizeof(progressText), "%d/%d", getLeaderboardPlayerTotal(i), MAX_PROGRESS_POINTS);
        drawPanelText(window, font, progressText, 20, sf::Color::White, panelX + panelWidth - 64.f, rowY + 2.f);
        char detailText[24];
        std::snprintf(detailText, sizeof(detailText), "L%d A%d", leaderboardLevelsCleared[i], leaderboardAchievementsUnlocked[i]);
        drawPanelText(window, font, detailText, 14, sf::Color(200, 200, 200), panelX + panelWidth - 62.f, rowY + 26.f);
    }
}


// Draws the achievement panel, progress bar, and unlock message.
void drawAchievementBoard(sf::RenderWindow& window, const sf::Font& font)
{
    const float panelX = -290.f, panelWidth = 280.f, top = 270.f;
    const char achDesc[ACHIEVEMENT_COUNT][32] = {
        "Reach 500 points", "Reach 1500 points", "Reach 3000 points",
        "Eat a ghost (red orb)", "Clear a level < 2 min"
    };

    int unlockedCount = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        if (achievementUnlocked[i]) unlockedCount++;

    drawPanelBox(window, panelX, top, panelWidth, 330.f);
    drawCenteredPanelText(window, font, "ACHIEVEMENTS", 26, sf::Color::Yellow, panelX + panelWidth / 2.f, top + 8.f);
    drawPanelLine(window, panelX + 10.f, top + 44.f, panelWidth - 20.f);

    char playerText[64];
    std::snprintf(playerText, sizeof(playerText), "PLAYER: %s", achievementPlayerName);
    drawPanelText(window, font, playerText, 18, sf::Color::White, panelX + 12.f, top + 52.f);
    char unlockedText[16];
    std::snprintf(unlockedText, sizeof(unlockedText), "%d/%d", unlockedCount, ACHIEVEMENT_COUNT);
    drawPanelText(window, font, unlockedText, 18, sf::Color(255, 215, 0), panelX + panelWidth - 50.f, top + 52.f);

    sf::RectangleShape barBack({ panelWidth - 24.f, 8.f });
    barBack.setPosition({ panelX + 12.f, top + 80.f });
    barBack.setFillColor(sf::Color(70, 70, 70));
    window.draw(barBack);
    sf::RectangleShape barFill({ (panelWidth - 24.f) * unlockedCount / ACHIEVEMENT_COUNT, 8.f });
    barFill.setPosition({ panelX + 12.f, top + 80.f });
    barFill.setFillColor(sf::Color(255, 215, 0));
    window.draw(barFill);

    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        float rowY = top + 100.f + i * 44.f;
        bool isUnlocked = achievementUnlocked[i];

        sf::RectangleShape check({ 20.f, 20.f });
        check.setPosition({ panelX + 14.f, rowY + 4.f });
        if (isUnlocked)
            check.setFillColor(sf::Color(255, 215, 0));
        else
            check.setFillColor(sf::Color(0, 0, 0, 0));
        check.setOutlineThickness(2.f);
        if (isUnlocked)
            check.setOutlineColor(sf::Color(255, 215, 0));
        else
            check.setOutlineColor(sf::Color(150, 150, 150));
        window.draw(check);
        if (isUnlocked)
            drawCenteredPanelText(window, font, "X", 16, sf::Color::Black, panelX + 24.f, rowY + 5.f);

        sf::Color achievementNameColor = sf::Color(170, 170, 170);
        sf::Color achievementDescriptionColor = sf::Color(120, 120, 120);
        if (isUnlocked)
        {
            achievementNameColor = sf::Color(255, 215, 0);
            achievementDescriptionColor = sf::Color(220, 220, 220);
        }
        drawPanelText(window, font, achievementNames[i], 20, achievementNameColor,
            panelX + 44.f, rowY);
        drawPanelText(window, font, achDesc[i], 14, achievementDescriptionColor,
            panelX + 44.f, rowY + 24.f);
    }

    if (achievementPopupTimer > 0)   // "UNLOCKED" popup maze ke upar
    {
        sf::Text pop(font, achievementPopupText, 36);
        pop.setFillColor(sf::Color::Yellow);
        pop.setOutlineThickness(3.f);
        pop.setOutlineColor(sf::Color::Black);
        pop.setPosition({ 200.f, 20.f });
        window.draw(pop);
    }
}
// ===== END OF PRETTY BOARDS =====
// ===== END OF ACHIEVEMENTS + TOP 3 PLAYERS =====

// ===== ADDING THESE TWO FUNCTIONS HERE COZ THEY ARE FOR MOVEMENT+DEFINED BOUNDARIES =====

// Returns true if a pixel position is outside the maze or inside a wall.
bool isWallAtPosition(float x, float y)
{
    if (x < 0 || y < 0)
        return true;

    int col = int(x) / TILE_SIZE;
    int row = int(y) / TILE_SIZE;

    if (col >= MAZE_COLUMNS || row >= MAZE_ROWS)
        return true;

    return mazeTiles[row * MAZE_COLUMNS + col] == 1;
}

// Checks whether a row and column point to a wall tile.
bool isWallTile(int row, int col)
{
    if (row < 0 || row >= MAZE_ROWS || col < 0 || col >= MAZE_COLUMNS)
        return false;
    return mazeTiles[row * MAZE_COLUMNS + col] == 1;
}

// Checks all four corners before allowing the player to move.
bool canPlayerMoveTo(float x, float y, float playerCollisionHalfSize)
{
    if (isWallAtPosition(x - playerCollisionHalfSize, y - playerCollisionHalfSize))
        return false;
    if (isWallAtPosition(x + playerCollisionHalfSize, y - playerCollisionHalfSize))
        return false;
    if (isWallAtPosition(x - playerCollisionHalfSize, y + playerCollisionHalfSize))
        return false;
    if (isWallAtPosition(x + playerCollisionHalfSize, y + playerCollisionHalfSize))
        return false;
    return true;
}

// Returns true if Pac-Man and a ghost share a clear row or column.
bool isGhostInClearLane(sf::Vector2f pacmanPos, sf::Vector2f ghostPos)
{
    const float alignmentTolerance = 3.f;

    if (pacmanPos.x < 0.f || pacmanPos.y < 0.f ||
        ghostPos.x < 0.f || ghostPos.y < 0.f ||
        pacmanPos.x >= MAZE_COLUMNS * TILE_SIZE || ghostPos.x >= MAZE_COLUMNS * TILE_SIZE ||
        pacmanPos.y >= MAZE_ROWS * TILE_SIZE || ghostPos.y >= MAZE_ROWS * TILE_SIZE)
        return false;

    int pacRow = (int)pacmanPos.y / TILE_SIZE;
    int pacCol = (int)pacmanPos.x / TILE_SIZE;
    int ghostRow = (int)ghostPos.y / TILE_SIZE;
    int ghostCol = (int)ghostPos.x / TILE_SIZE;

    // Same horizontal lane.
    if (pacRow == ghostRow && std::abs(pacmanPos.y - ghostPos.y) <= alignmentTolerance)
    {
        int firstCol = pacCol;
        int lastCol = ghostCol;
        if (ghostCol < pacCol)
        {
            firstCol = ghostCol;
            lastCol = pacCol;
        }
        firstCol++;

        for (int col = firstCol; col < lastCol; col++)
        {
            if (mazeTiles[pacRow * MAZE_COLUMNS + col] == 1)
                return false;
        }
        return true;
    }

    // Same vertical lane.
    if (pacCol == ghostCol && std::abs(pacmanPos.x - ghostPos.x) <= alignmentTolerance)
    {
        int firstRow = pacRow;
        int lastRow = ghostRow;
        if (ghostRow < pacRow)
        {
            firstRow = ghostRow;
            lastRow = pacRow;
        }
        firstRow++;

        for (int row = firstRow; row < lastRow; row++)
        {
            if (mazeTiles[row * MAZE_COLUMNS + pacCol] == 1)
                return false;
        }
        return true;
    }

    return false;
}
//leaderboard code by hmza:

char scorePlayerNames[6][PLAYER_NAME_CAPACITY] = {};    // player name of each run
int scoreValues[6];           // 5 best scores + 1 spare slot
int scoreRunTimes[6];            // seconds each run lasted
std::time_t scoreTimeStamps[6];   // when each run ended (unix time)
int savedScoreCount = 0;         // how many scores we have right now

// Changes a saved time into readable date and time text.
void formatScoreDateAndTime(std::time_t t, char dateText[])
{
    std::tm tmv = {};
#ifdef _WIN32
    localtime_s(&tmv, &t);
#else
    localtime_r(&t, &tmv);
#endif
    std::strftime(dateText, 32, "%d/%m %H:%M", &tmv);
}

// Loads saved scores, run times, and dates when the game starts.
void loadScoreLeaderboard()
{
    std::ifstream file("leaderboard_v2.txt");
    savedScoreCount = 0;
    long long stamp = 0;
    while (savedScoreCount < 5)
    {
        file.width(PLAYER_NAME_CAPACITY);
        if (!(file >> scorePlayerNames[savedScoreCount] >> scoreValues[savedScoreCount]
            >> scoreRunTimes[savedScoreCount] >> stamp))
            break;
        scoreTimeStamps[savedScoreCount] = (std::time_t)stamp;
        savedScoreCount++;
    }
}

// Adds a score, sorts the list, keeps the top five, and saves it.
void addScoreToLeaderboard(const char name[], int score, int seconds)
{
    std::snprintf(scorePlayerNames[savedScoreCount], sizeof(scorePlayerNames[savedScoreCount]), "%s", name);
    scoreValues[savedScoreCount] = score;
    scoreRunTimes[savedScoreCount] = seconds;
    scoreTimeStamps[savedScoreCount] = std::time(nullptr);   // time stamp of this run
    savedScoreCount++;

    // sort: biggest score first (bubble sort)
    for (int i = 0; i < savedScoreCount - 1; i++)
    {
        for (int j = 0; j < savedScoreCount - 1 - i; j++)
        {
            if (scoreValues[j] < scoreValues[j + 1])
            {
                int temp = scoreValues[j];
                scoreValues[j] = scoreValues[j + 1];
                scoreValues[j + 1] = temp;

                temp = scoreRunTimes[j];
                scoreRunTimes[j] = scoreRunTimes[j + 1];
                scoreRunTimes[j + 1] = temp;

                char nameCopy[PLAYER_NAME_CAPACITY];
                std::snprintf(nameCopy, sizeof(nameCopy), "%s", scorePlayerNames[j]);
                std::snprintf(scorePlayerNames[j], sizeof(scorePlayerNames[j]), "%s", scorePlayerNames[j + 1]);
                std::snprintf(scorePlayerNames[j + 1], sizeof(scorePlayerNames[j + 1]), "%s", nameCopy);

                std::time_t ts = scoreTimeStamps[j];
                scoreTimeStamps[j] = scoreTimeStamps[j + 1];
                scoreTimeStamps[j + 1] = ts;
            }
        }
    }

    if (savedScoreCount > 5)
        savedScoreCount = 5;                  // forget the 6th

    // save to the file
    std::ofstream file("leaderboard_v2.txt");
    for (int i = 0; i < savedScoreCount; i++)
        file << scorePlayerNames[i] << " " << scoreValues[i] << " " << scoreRunTimes[i] << " "
        << (long long)scoreTimeStamps[i] << "\n";
}

// Builds one readable row for the high-score list.
void formatScoreLeaderboardLine(int scoreIndex, char line[], std::size_t lineSize)
{
    char dateText[32];
    formatScoreDateAndTime(scoreTimeStamps[scoreIndex], dateText);
    std::snprintf(line, lineSize, "%d.  %s  %d pts  %d sec  %s",
        scoreIndex + 1, scorePlayerNames[scoreIndex], scoreValues[scoreIndex],
        scoreRunTimes[scoreIndex], dateText);
}

// Draws the saved high scores in the center of the screen.
void drawScoreLeaderboard(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title(font, "TOP 5 SCORES", 40);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin({ tb.position.x + tb.size.x / 2.f, 0.f });   // horizontal centre
    title.setPosition({ 600.f, 460.f });
    window.draw(title);

    // find the widest row so the whole list is centred as one block
    float maxW = 0.f;
    for (int i = 0; i < savedScoreCount; i++)
    {
        char line[128];
        formatScoreLeaderboardLine(i, line, sizeof(line));
        sf::Text row(font, line, 26);
        float w = row.getLocalBounds().size.x;
        if (w > maxW)
            maxW = w;
    }

    for (int i = 0; i < savedScoreCount; i++)
    {
        char line[128];
        formatScoreLeaderboardLine(i, line, sizeof(line));
        sf::Text row(font, line, 26);
        row.setPosition({ 600.f - maxW / 2.f, 520.f + i * 40.f });
        window.draw(row);
    }
}

// Plays the next available sound in a playlist without overlapping its sounds.
void playNextSound(sf::Sound* sounds[], const bool loaded[],
    int soundCount, int& nextIndex)
{
    for (int attempt = 0; attempt < soundCount; attempt++)
    {
        int soundIndex = nextIndex;
        nextIndex = (nextIndex + 1) % soundCount;
        if (!loaded[soundIndex])
            continue;

        for (int i = 0; i < soundCount; i++)
            sounds[i]->stop();

        sounds[soundIndex]->play();
        return;
    }
}

// Checks whether music or another sound effect is playing.
bool isOtherAudioPlaying(sf::Music& menuMusic, sf::Sound* laneSounds[],
    sf::Sound* deathSounds[])
{
    if (menuMusic.getStatus() == sf::SoundSource::Status::Playing)
        return true;

    for (int i = 0; i < 4; i++)
        if (laneSounds[i]->getStatus() == sf::SoundSource::Status::Playing)
            return true;

    for (int i = 0; i < 3; i++)
        if (deathSounds[i]->getStatus() == sf::SoundSource::Status::Playing)
            return true;

    return false;
}

// Sets up the game and runs its menus, gameplay, and drawing.
int main()
{
    // seed the random numbers (ghost decisions, orb spawning)
    std::srand((unsigned int)std::time(nullptr));

    // window
    sf::RenderWindow window(sf::VideoMode({ 1500, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);
    // added by abd for left achievement board
    sf::View gameView(sf::FloatRect({ -300.f, 0.f }, { 1500.f, 900.f }));   // left panel ke liye 300px extra
    window.setView(gameView);

    // maze background
    sf::Texture mazeTexture("maze.bmp");
    sf::Sprite mazeBackground(mazeTexture);

    // player
    sf::Texture pacmanTextureOpen("Player1_TextureA.png");
    sf::Texture pacmanTextureClosed("Player1_TextureB.png");
    Player player(pacmanTextureOpen);
    player.sprite.setScale(
        {
            40 / float(pacmanTextureOpen.getSize().x),
            40 / float(pacmanTextureOpen.getSize().y)
        });
    float playerWidth = player.sprite.getLocalBounds().size.x;
    float playerHeight = player.sprite.getLocalBounds().size.y;
    player.sprite.setOrigin({ playerWidth / 2, playerHeight / 2 });
    player.sprite.setPosition({ 54, 846 });

    // this code is for making the ghost
        // this code is for making red ghost blinky
    sf::Texture redGhostTexture("red_ghost.png");
    Ghost redGhost(redGhostTexture);
    redGhost.sprite.setScale({
        30 / float(redGhostTexture.getSize().x),
        30 / float(redGhostTexture.getSize().y)
        });
    redGhost.sprite.setPosition({ 400,400 });
    //i,hamza writing this code to add blue ghosts in the game.

    sf::Texture blueGhostTexture("bluee.png");
    Ghost blueGhost(blueGhostTexture);
    blueGhost.sprite.setScale({
        36 / float(blueGhostTexture.getSize().x),
        36 / float(blueGhostTexture.getSize().y)
        });
    blueGhost.sprite.setPosition({ 400, 360 });

    //i,hamza writing this code to add yellow ghosts in the game.

    sf::Texture yellowGhostTexture("yellow_ghost.png");
    Ghost yellowGhost(yellowGhostTexture);
    yellowGhost.sprite.setScale({
        30 / float(yellowGhostTexture.getSize().x),
        30 / float(yellowGhostTexture.getSize().y)
        });
    yellowGhost.sprite.setPosition({ 470, 400 });

    // this code is for making pink ghost pinky.
    sf::Texture pinkGhostTexture("pink_ghost.png");
    Ghost pinkGhost(pinkGhostTexture);
    pinkGhost.sprite.setScale({
        36 / float(pinkGhostTexture.getSize().x),
        36 / float(pinkGhostTexture.getSize().y)
        });
    pinkGhost.sprite.setPosition({ 470, 360 });
    // adding new ghosts for next levels by abd
    Ghost extraRedGhost(redGhostTexture), extraPinkGhost(pinkGhostTexture), extraBlueGhost(blueGhostTexture), extraYellowGhost(yellowGhostTexture);

    // this makes the pasue menu screen
    sf::RectangleShape overlay({ 900, 900 });
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Font font;
    if (!font.openFromFile("pixel_font.otf"))
        return -1;

    sf::Text paused_text(font, "GAME PAUSED", 100);
    paused_text.setPosition({ 230, 330 });
    sf::Text paused_text_additional(font, "Press Esc or P to continue", 30);
    paused_text_additional.setPosition({ 290, 480 });



    // making the sidebar which tells game name and score
    sf::Texture sidebarTexture("Game_banner.jpg");
    sf::Sprite rightSidebar(sidebarTexture);
    rightSidebar.setPosition({ 900,0 });
    rightSidebar.setScale({ 300.f / sidebarTexture.getSize().x, 900.f / sidebarTexture.getSize().y });
    // abdullahameer added achievement board here
    sf::Sprite leftSidebar(sidebarTexture);
    leftSidebar.setPosition({ -300.f, 0.f });
    leftSidebar.setScale({ 300.f / sidebarTexture.getSize().x, 900.f / sidebarTexture.getSize().y });
    sf::RectangleShape leftDark({ 300.f, 900.f });          // text saaf parhne ke liye halka andhera
    leftDark.setPosition({ -300.f, 0.f });
    leftDark.setFillColor(sf::Color(0, 0, 0, 170));
    //
    sf::Text gameTitle(font, "PACMAN", 100);
    gameTitle.setFillColor(sf::Color::Yellow);
    gameTitle.setPosition({ 910, 200 });

    sf::Text scoreLabel(font, "Score: ", 50);
    scoreLabel.setFillColor(sf::Color::Red);
    scoreLabel.setPosition({ 950,300 });

    sf::Text scoreDisplay(font, "0", 50);
    scoreDisplay.setFillColor(sf::Color::Red);
    scoreDisplay.setPosition({ 1080, 300 });

    sf::Text livesLabel(font, "Lives Left: ", 50);
    livesLabel.setFillColor(sf::Color::Red);
    livesLabel.setPosition({ 920, 350 });

    sf::Text livesDisplay(font, "0", 50);
    livesDisplay.setFillColor(sf::Color::Red);
    livesDisplay.setPosition({ 1130,350 });


    // Making the game over screen
    sf::RectangleShape game_over_screen({ 1200,900 });
    game_over_screen.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Text game_over_text(font, "GAME OVER", 200);
    sf::FloatRect go_b = game_over_text.getLocalBounds();
    game_over_text.setOrigin({ go_b.position.x + go_b.size.x / 2.f, 0.f });   // horizontal centre
    game_over_text.setPosition({ 600.f, 200.f });

    sf::Text game_over_text_additional(font, "Press Space to start again", 50);
    sf::FloatRect goa_b = game_over_text_additional.getLocalBounds();
    game_over_text_additional.setOrigin({ goa_b.position.x + goa_b.size.x / 2.f, 0.f });
    game_over_text_additional.setPosition({ 600.f, 400.f });



    // variables (each declared ONCE)
    sf::Clock clock, timer_for_texture_change;
    float playerCollisionHalfSize = 15;         // half of Pac-Man's size (40 / 2) //another change here half=15 from now to make pac-man movement checking defined.
    char playerDirection = ' ';
    char requestedDirection = ' ';       // direction the player asked for (turn happens at the next tile centre)
    bool hasRunStarted = false, showOpenMouth = false;
    bool isGamePaused = false;

    //seconds counter for leaderboard: 

    float runTimeSeconds = 0;           // seconds played in this run
    bool scoreWasSaved = false;     // so we save only once per game over
    loadScoreLeaderboard();                 // read the file when the game starts

    // frenzy mode (5.5) by hmza
    bool frenzyIsActive = false;      // true while frenzy is running
    float timeUntilFrenzy = 0;       // seconds played since the last frenzy
    float frenzyTimeLeft = 0;       // seconds of frenzy still left
    // places dots in every 1x1 area
    //Pinky.alive = blue.alive = false;   // TEST: 2 ghosts pehle se mare hue   // testing for boss
    placeMazeDots();

    // ===== GHOST MOVEMENT SETUP =====
    // Done by MUHAMMAD KHIZAR REHMAN
    // edit by abd
    Ghost* ghosts[8] = { &redGhost, &pinkGhost, &blueGhost, &yellowGhost, &extraRedGhost, &extraPinkGhost, &extraBlueGhost, &extraYellowGhost };
    // edited by abd for makking ghost color same for some time 
    sf::Texture* ghostTextures[8] = { &redGhostTexture, &pinkGhostTexture, &blueGhostTexture, &yellowGhostTexture, &redGhostTexture, &pinkGhostTexture, &blueGhostTexture, &yellowGhostTexture };
    float ghostSizes[8] = { 30, 36, 36, 30, 30, 36, 36, 30 }; // har ghost ka size (jo upar setScale mein diya tha) aur next levels ke liye bhi ghosts
    bool ghostsScared = false;
    // edit by abd
    int ghostRows[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile the ghost is on
    int ghostColumns[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // column of that tile
    int ghostTargetRows[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile it is walking to
    int ghostTargetColumns[8] = { 11, 13, 11, 13 ,11, 13, 11, 13 };   // column of that tile
    int ghostDirectionRows[8] = { 0, 0, 0, 0,0, 0, 0, 0 };       // last step taken (row)
    int ghostDirectionColumns[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };       // last step taken (column)
    int ghostCornerRows[8] = { 1, 1, 23, 23,1, 1, 23, 23 };     // corner each ghost runs to (row)
    int ghostCornerColumns[8] = { 23, 1, 23, 1, 23, 1, 23, 1 };     // corner each ghost runs to (column)
    int rowDirections[8] = { -1, 0, 1, 0,-1, 0, 1, 0 };      // the 4 ways: up, left, down, right
    int columnDirections[8] = { 0, -1, 0, 1, 0, -1, 0, 1 };
    float ghostReleaseTimes[8] = { 0, 3, 6, 9, 12, 15, 18, 21 };    // Done by MUHAMMAD KHIZAR REHMAN: every ghost has its own release time
    float ghostGameTime = 0, ghostModeTime = 0;             // game time, mode timer
    bool ghostScatterMode = false;                // false = chase, true = run to corner

    // ===== LIFE LOSS SETUP (ghost touches pac-man) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool resetGhosts = false;                 // true = put ghosts back in the house
    int ghostHomeRows[8] = { 11, 10, 10, 11,11, 10, 10, 11 };   // home tile of each ghost (row)
    int ghostHomeColumns[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // home tile of each ghost (column)

    // ===== GATES SETUP (5.6) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    int entryGateRow = 23, entryGateColumn = 1;       // entry gate tile (lower left corner)
    int exitGateRow = 1, exitGateColumn = 23;       // exit gate tile (upper right corner)
    int pelletsCollected = 0;               // pellets eaten so far this level
    int totalPellets = 0;               // pellets that were on the stage at the start
    for (int i = 0; i < MAZE_COLUMNS * MAZE_ROWS; i++)
        if (mazeTiles[i] == 2)
            totalPellets++;
    float pelletRespawnTimers[MAZE_COLUMNS * MAZE_ROWS] = { 0 };   // respawn countdown of each eaten pellet
    float pelletRespawnSeconds = 300.f;               // an eaten pellet comes back after 300 sec (5 min)
    sf::RectangleShape gateShape({ 36.f, 36.f });
    gateShape.setOutlineThickness(3.f);
    gateShape.setOutlineColor(sf::Color::White);
    sf::Text gateInfoText(font, "", 25);
    gateInfoText.setFillColor(sf::Color::White);
    gateInfoText.setPosition({ 920.f, 600.f });

    // ===== GRAPE SETUP (5.10 extra life) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool grapeIsVisible = false;        // is the grape on the map right now?
    float timeSinceGrapeSpawn = 0;          // seconds passed since the last grape spawn
    float grapeSpawnInterval = 300;        // spawn every 300 sec (5 min)
    sf::Texture grapeTexture("grape.png");         // grape picture (hmza: image instead of the purple circle)
    sf::Sprite grapeSprite(grapeTexture);
    grapeSprite.setScale({ 30.f / grapeTexture.getSize().x, 30.f / grapeTexture.getSize().y });   // 30 x 30 pixels on screen
    grapeSprite.setOrigin({ grapeTexture.getSize().x / 2.f, grapeTexture.getSize().y / 2.f });    // position = middle of the grape
    grapeSprite.setPosition({ 450.f, 306.f });          // square just outside the ghost house door

    // ===== SPECIAL BONUS ORB SETUP (5.2) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    int bonusOrbRow = 8, bonusOrbColumn = 11;       // bonus orb tile: just outside the ghost house door, right next to the grape
    bool bonusOrbIsVisible = true;            // is the bonus orb on the map right now?
    float bonusOrbRespawnTimer = 0;              // seconds passed since the bonus orb was eaten
    float bonusOrbRespawnSeconds = 120;           // it comes back 120 sec (2 min) after it was eaten
    sf::CircleShape bonusOrbShape(12.f);
    bonusOrbShape.setFillColor(sf::Color(255, 120, 0));        // orange
    bonusOrbShape.setOutlineThickness(2.f);
    bonusOrbShape.setOutlineColor(sf::Color::White);
    bonusOrbShape.setOrigin({ 12.f, 12.f });                   // position = middle of the orb
    bonusOrbShape.setPosition({ (bonusOrbColumn + 0.5f) * TILE_SIZE, (bonusOrbRow + 0.5f) * TILE_SIZE });

    // edit by abd
    for (int i = 0; i < 8; i++)
    {
        ghosts[i]->sprite.setTexture(*ghostTextures[i], true);
        ghosts[i]->sprite.setScale({ ghostSizes[i] / float(ghostTextures[i]->getSize().x), ghostSizes[i] / float(ghostTextures[i]->getSize().y) });
        ghosts[i]->sprite.setOrigin({ ghostTextures[i]->getSize().x / 2.f, ghostTextures[i]->getSize().y / 2.f });
        ghosts[i]->sprite.setPosition({ (ghostColumns[i] + 0.5f) * TILE_SIZE, (ghostRows[i] + 0.5f) * TILE_SIZE });
    }

    // ===== END OF GHOST MOVEMENT SETUP =====


    // this makes sure that holding esc dosent make pause screen flicker
    window.setKeyRepeatEnabled(false);
    //abdullahameer coded here
        // ===== PLAYER NAME SCREEN (Abdullah Ameer) =====
    char playerName[PLAYER_NAME_CAPACITY] = "";
    int playerNameLength = 0;
    bool nameEntered = false;

    sf::Text nameTitle(font, "ENTER YOUR NAME", 70);
    nameTitle.setFillColor(sf::Color::Yellow);
    nameTitle.setPosition({ 250.f, 280.f });
    sf::Text nameText(font, "", 60);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition({ 250.f, 400.f });
    sf::Text nameHint(font, "Type name and press Enter", 30);
    nameHint.setFillColor(sf::Color(180, 180, 180));
    nameHint.setPosition({ 250.f, 520.f });

    // ===== AUDIO SETUP =====
    // Pellet-eating sound. It is retriggered whenever Pac-Man collects a pellet.
    sf::SoundBuffer pelletSoundBuffer;
    bool pelletSoundLoaded = pelletSoundBuffer.loadFromFile(
        "Pacman+-+Eating+(Sound+Effect).mp3");
    sf::Sound pelletSound(pelletSoundBuffer);
    pelletSound.setVolume(50.f); // 50% volume
    if (!pelletSoundLoaded)
        std::cerr << "Could not load Pacman+-+Eating+(Sound+Effect).mp3. "
        << "Check the file and its path.\n";

    // Menu/name-entry background music.
    sf::Music menuMusic;
    bool menuMusicLoaded = menuMusic.openFromFile(
        "breakzstudios-electronic-uplifting-retro-game-hidden-gem-short-version-210310.mp3");
    if (menuMusicLoaded)
    {
        menuMusic.setLooping(true);
        menuMusic.setVolume(65.f);
        menuMusic.play();
    }
    else
    {
        std::cerr << "Could not load menu music. Check the MP3 file and its path.\n";
    }

    // Chase/lane-warning playlist. The sounds play in this order and then
    // the playlist starts again from the first sound:
    // pakad-mc.mp3 -> aree-cid.mp3 -> cid-maiya-chood.mp3
    // -> cid-aak-bar-mil-jabeta.mp3 -> repeat.
    sf::SoundBuffer laneSoundBuffer0;
    sf::SoundBuffer laneSoundBuffer1;
    sf::SoundBuffer laneSoundBuffer2;
    sf::SoundBuffer laneSoundBuffer3;

    bool laneSoundLoaded[4] = {
        laneSoundBuffer0.loadFromFile("pakad-mc.mp3"),
        laneSoundBuffer1.loadFromFile("aree-cid.mp3"),
        laneSoundBuffer2.loadFromFile("cid-maiya-chood.mp3"),
        laneSoundBuffer3.loadFromFile("cid-aak-bar-mil-jabeta.mp3")
    };

    sf::Sound laneWarningSound0(laneSoundBuffer0);
    sf::Sound laneWarningSound1(laneSoundBuffer1);
    sf::Sound laneWarningSound2(laneSoundBuffer2);
    sf::Sound laneWarningSound3(laneSoundBuffer3);
    sf::Sound* laneWarningSounds[4] = {
        &laneWarningSound0, &laneWarningSound1,
        &laneWarningSound2, &laneWarningSound3
    };

    const char laneSoundFiles[4][40] = {
        "pakad-mc.mp3", "aree-cid.mp3",
        "cid-maiya-chood.mp3", "cid-aak-bar-mil-jabeta.mp3"
    };
    for (int i = 0; i < 4; i++)
        if (!laneSoundLoaded[i])
            std::cerr << "Could not load " << laneSoundFiles[i]
            << ". Check the file and its path.\n";


    sf::SoundBuffer deathSoundBuffer0;
    sf::SoundBuffer deathSoundBuffer1;
    sf::SoundBuffer deathSoundBuffer2;

    bool deathSoundLoaded[3] = {
        deathSoundBuffer0.loadFromFile("ghost3.mp3.mpeg"),
        deathSoundBuffer1.loadFromFile("cid-acp-pradhuman-fasi.mp3"),
        deathSoundBuffer2.loadFromFile("cid-ke-bakchodi.mp3")
    };

    sf::Sound deathSound0(deathSoundBuffer0);
    sf::Sound deathSound1(deathSoundBuffer1);
    sf::Sound deathSound2(deathSoundBuffer2);
    sf::Sound* deathSounds[3] = {
        &deathSound0, &deathSound1, &deathSound2
    };

    const char deathSoundFiles[3][48] = {
        "ghost3.mp3.mpeg", "cid-acp-pradhuman-fasi.mp3",
        "cid-ke-bakchodi.mp3"
    };
    for (int i = 0; i < 3; i++)
        if (!deathSoundLoaded[i])
            std::cerr << "Could not load " << deathSoundFiles[i]
            << ". Check the file and its path.\n";

    // Each playlist remembers which sound should play next.
    int nextLaneSound = 0;
    int nextDeathSound = 0;

    // Prevent the lane warning from restarting every frame while a ghost
    // remains in the same unobstructed row or column as Pac-Man.
    bool ghostLaneWasActive = false;

    // ===== END AUDIO SETUP =====

    // Let the player enter a name before starting the game.
    while (window.isOpen() && !nameEntered)
    {
        std::optional<sf::Event> event = window.pollEvent();
        while (event.has_value())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            const sf::Event::TextEntered* textEntered = event->getIf<sf::Event::TextEntered>();
            if (textEntered != nullptr)
            {
                char enteredCharacter = 0;
                if (textEntered->unicode < 128)
                    enteredCharacter = (char)textEntered->unicode;
                if (textEntered->unicode == 8)
                {
                    if (playerNameLength > 0)
                    {
                        playerNameLength--;
                        playerName[playerNameLength] = '\0';
                    }
                }
                else if (playerNameLength < PLAYER_NAME_LIMIT &&
                    ((enteredCharacter >= 'a' && enteredCharacter <= 'z') ||
                        (enteredCharacter >= 'A' && enteredCharacter <= 'Z') ||
                        (enteredCharacter >= '0' && enteredCharacter <= '9')))
                {
                    playerName[playerNameLength] = enteredCharacter;
                    playerNameLength++;
                    playerName[playerNameLength] = '\0';
                }
            }

            const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>();
            if (keyPressed != nullptr)
            {
                if (keyPressed->code == sf::Keyboard::Key::Enter && playerName[0] != '\0')
                    nameEntered = true;
            }
            event = window.pollEvent();
        }

        char enteredNameText[PLAYER_NAME_CAPACITY + 2];
        std::snprintf(enteredNameText, sizeof(enteredNameText), "%s_", playerName);
        nameText.setString(enteredNameText);
        window.clear();
        window.draw(nameTitle);
        window.draw(nameText);
        window.draw(nameHint);
        window.display();
    }

    // The menu/name-entry music stops as soon as the game starts.
    menuMusic.stop();

    if (playerName[0] == '\0')
        std::snprintf(playerName, sizeof(playerName), "PLAYER");
    loadPlayerAchievements(playerName);
    loadProgressLeaderboard();
    updateProgressLeaderboard(achievementPlayerName, 0);
    clock.restart();
    // ===== END OF PLAYER NAME SCREEN =====

    // Keep handling game input, movement, scoring, and drawing until the window closes.
    while (window.isOpen())
    {
        // This makes sure you can pause game or exit window
        std::optional<sf::Event> event = window.pollEvent();
        while (event.has_value())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
                    isGamePaused = !isGamePaused;
            }
            event = window.pollEvent();
        }

        // first check if player has enough lives, if he does run game else show Game over screen
        if (player.lives > 0)
        {
            // check is game paused or not
            if (!isGamePaused)
            {
                // this variable stores the time between 2 movement key presses
                float deltaTime = clock.restart().asSeconds();
                if (deltaTime > 0.05f)
                    deltaTime = 0.05f;   // Done by MUHAMMAD KHIZAR REHMAN: stops the big jump after pause

                // this variable stores the total time of the game run: (leaderboard code by hmza):
                runTimeSeconds += deltaTime;

                // frenzy mode (5.5) by hmza
                if (hasRunStarted)
                {
                    if (frenzyIsActive)
                    {
                        frenzyTimeLeft -= deltaTime;
                        if (frenzyTimeLeft <= 0)      // 10 seconds over
                        {
                            frenzyIsActive = false;
                            timeUntilFrenzy = 0;       // start counting the next minute
                        }
                    }
                    else
                    {
                        timeUntilFrenzy += deltaTime;
                        if (timeUntilFrenzy >= 60)     // 1 minute passed
                        {
                            frenzyIsActive = true;
                            frenzyTimeLeft = 10;
                        }
                    }
                }
                int scoreMultiplier = 1;
                if (frenzyIsActive)
                    scoreMultiplier = 5;      // points multiplier
                // editimg by abd
                updatePowerUps(deltaTime);
                //abd
                updateAchievementPopup(deltaTime);
                if (levelBannerTimer > 0)
                    levelBannerTimer -= deltaTime;

                // ===== GRID-LOCKED MOVEMENT (pac-man moves only along a single row / column) =====
                // keys only set the WANTED direction
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    requestedDirection = 'U';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    requestedDirection = 'L';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    requestedDirection = 'D';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    requestedDirection = 'R';

                sf::Vector2f playerPosition = player.sprite.getPosition();
                float step = player.speed * deltaTime * getPlayerSpeedMultiplier();
                int row = int(playerPosition.y) / TILE_SIZE;
                int col = int(playerPosition.x) / TILE_SIZE;
                float cx = (col + 0.5f) * TILE_SIZE;   // centre of current tile
                float cy = (row + 0.5f) * TILE_SIZE;

                // turn only if that way is open and pac-man is at the tile centre (reversing is always allowed)
                if (requestedDirection != ' ' && requestedDirection != playerDirection)
                {
                    bool reverse = (playerDirection == 'U' && requestedDirection == 'D') || (playerDirection == 'D' && requestedDirection == 'U') ||
                        (playerDirection == 'L' && requestedDirection == 'R') || (playerDirection == 'R' && requestedDirection == 'L');
                    int tr = row + (requestedDirection == 'D') - (requestedDirection == 'U');
                    int tc = col + (requestedDirection == 'R') - (requestedDirection == 'L');
                    bool atCentre = std::abs(playerPosition.x - cx) <= step && std::abs(playerPosition.y - cy) <= step;

                    if (!isWallTile(tr, tc) && (reverse || atCentre))
                    {
                        if (!reverse)
                            playerPosition = { cx, cy };          // snap into the new lane
                        playerDirection = requestedDirection;
                        hasRunStarted = true;
                    }
                }

                // move along the current lane only
                float newX = playerPosition.x, newY = playerPosition.y;
                bool horizontal = (playerDirection == 'L' || playerDirection == 'R');

                if (playerDirection == 'U') { player.sprite.setRotation(sf::degrees(270)); newY -= step; }
                else if (playerDirection == 'L') { player.sprite.setRotation(sf::degrees(180)); newX -= step; }
                else if (playerDirection == 'D') { player.sprite.setRotation(sf::degrees(90));  newY += step; }
                else if (playerDirection == 'R') { player.sprite.setRotation(sf::degrees(360)); newX += step; }

                if (playerDirection != ' ')
                {
                    if (canPlayerMoveTo(newX, newY, playerCollisionHalfSize))
                        playerPosition = { newX, newY };
                    else if (horizontal)
                        playerPosition.x = cx;                    // hit a wall: stop at tile centre
                    else
                        playerPosition.y = cy;

                    if (horizontal) playerPosition.y = cy;        // lock to the row
                    else            playerPosition.x = cx;        // lock to the column
                }

                // starts the pacman animation if any movement key is pressed
                if (hasRunStarted)
                {
                    // write code for changing of texture
                    if (timer_for_texture_change.getElapsedTime().asSeconds() >= 0.3)
                    {
                        showOpenMouth = !showOpenMouth;
                        if (showOpenMouth)
                            player.sprite.setTexture(pacmanTextureOpen);
                        else
                            player.sprite.setTexture(pacmanTextureClosed);
                        timer_for_texture_change.restart();
                    }
                }

                player.sprite.setPosition(playerPosition);
                // ===== END GRID-LOCKED MOVEMENT =====


                // Check every maze tile for pellets close enough to collect.
                for (int r = 0; r < MAZE_ROWS; r++)
                {
                    for (int c = 0; c < MAZE_COLUMNS; c++)
                    {
                        if (mazeTiles[r * MAZE_COLUMNS + c] == 2)
                        {
                            float dx = playerPosition.x - (c + 0.5f) * TILE_SIZE;
                            float dy = playerPosition.y - (r + 0.5f) * TILE_SIZE;
                            if (dx * dx + dy * dy < 15.f * 15.f)
                            {
                                mazeTiles[r * MAZE_COLUMNS + c] = 0;
                                if (pelletSoundLoaded &&
                                    !isOtherAudioPlaying(menuMusic, laneWarningSounds, deathSounds))
                                {
                                    // Pellet audio is only allowed when no other
                                    // audio is playing. It is kept at 50% volume.
                                    pelletSound.stop();
                                    pelletSound.play();
                                }
                                player.score += 10 * scoreMultiplier;
                                pelletsCollected++;                        // Done by MUHAMMAD KHIZAR REHMAN: count this pellet
                                pelletRespawnTimers[r * MAZE_COLUMNS + c] = pelletRespawnSeconds;       // Done by MUHAMMAD KHIZAR REHMAN: it will come back later
                            }
                        }
                    }
                }
                // orbits  and levels speed added by abdullahameer
                player.score += collectPowerUps(playerPosition) * scoreMultiplier;
                checkScoreAchievements(player.score);

                // ===== PELLET RESPAWN + EXIT GATE (5.6) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                for (int k = 0; k < MAZE_COLUMNS * MAZE_ROWS; k++)
                {
                    if (pelletRespawnTimers[k] > 0)
                    {
                        pelletRespawnTimers[k] -= deltaTime;
                        if (pelletRespawnTimers[k] <= 0 && mazeTiles[k] == 0)
                            mazeTiles[k] = 2;                  // the pellet is back
                    }
                }
                float qx = playerPosition.x - (exitGateColumn + 0.5f) * TILE_SIZE;
                float qy = playerPosition.y - (exitGateRow + 0.5f) * TILE_SIZE;

                // ===== LEVEL UP (6.1, 6.2) =====
                // Done by MUHAMMAD KHIZAR REHMAN: the level is now cleared at the open exit gate (was: dotsLeft == 0)
                if (pelletsCollected >= totalPellets && qx * qx + qy * qy < 18.f * 18.f)
                {
                    currentLevel++;
                    // edited by abd
                    checkFastLevelAchievement(runTimeSeconds - levelStartTime);
                    levelStartTime = runTimeSeconds;
                    updateProgressLeaderboard(achievementPlayerName, currentLevel - 1);   // levels cleared so far
                    //done
                    levelSpeedMultiplier *= 1.10f;                        // ghosts aur Pacman dono tez
                    if (activeGhostCount < MAX_GHOST_COUNT)
                        activeGhostCount++;  // 1 ghost zyada
                    levelBannerTimer = 2.5f;                         // "LEVEL n" 2.5 sec dikhao

                    placeMazeDots();
                    player.score -= 10;                        // start tile ka dot dobara khaya jata hai
                    player.sprite.setPosition({ 54, 846 });
                    playerPosition = player.sprite.getPosition();
                    playerDirection = ' ';
                    requestedDirection = ' ';
                    resetPowerUps(true);
                    resetGhosts = true;                                 // ghosts wapas house mein

                    pelletsCollected = 0;                                    // Done by MUHAMMAD KHIZAR REHMAN: new level, new pellet count
                    for (int k = 0; k < MAZE_COLUMNS * MAZE_ROWS; k++)
                        pelletRespawnTimers[k] = 0;

                    bonusOrbIsVisible = true;                                  // Done by MUHAMMAD KHIZAR REHMAN: bonus orb is back on a new level
                    bonusOrbRespawnTimer = 0;

                    for (int i = 0; i < 8; i++)                 // sab ghosts zinda aur normal
                    {
                        ghosts[i]->alive = true;
                        ghosts[i]->isBoss = false;
                        ghosts[i]->speed = 130;
                        ghosts[i]->sprite.setColor(sf::Color::White);
                        ghosts[i]->sprite.setTexture(*ghostTextures[i], true);
                        ghosts[i]->sprite.setScale({ ghostSizes[i] / ghostTextures[i]->getSize().x, ghostSizes[i] / ghostTextures[i]->getSize().y });
                        ghosts[i]->sprite.setOrigin({ ghostTextures[i]->getSize().x / 2.f, ghostTextures[i]->getSize().y / 2.f });
                    }
                    ghostsScared = false;
                }
                // ===== END LEVEL UP =====

                // kill-ghost orb chal raha ho to sab ghosts ek jese, khatam ho to wapas asli
                bool scaredNow = isGhostKillPowerActive();
                if (scaredNow != ghostsScared)
                {
                    ghostsScared = scaredNow;
                    for (int i = 0; i < 8; i++)
                    {
                        if (ghosts[i]->isBoss)
                            continue;                 // boss keeps its red boss look
                        const sf::Texture* textureToUse = ghostTextures[i];
                        if (ghostsScared)
                            textureToUse = &blueGhostTexture;
                        ghosts[i]->sprite.setTexture(*textureToUse, true);
                        ghosts[i]->sprite.setScale({
                            ghostSizes[i] / float(textureToUse->getSize().x),
                            ghostSizes[i] / float(textureToUse->getSize().y)
                            });
                        ghosts[i]->sprite.setOrigin({
                            textureToUse->getSize().x / 2.f,
                            textureToUse->getSize().y / 2.f
                            });
                    }
                }



                // Move each active ghost toward its next target tile.
                // ===== GHOST MOVEMENT =====
                // Done by MUHAMMAD KHIZAR REHMAN

                // after a hit, put the ghosts back in the house and restart their timers
                if (resetGhosts)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        ghostRows[i] = ghostTargetRows[i] = ghostHomeRows[i];
                        ghostColumns[i] = ghostTargetColumns[i] = ghostHomeColumns[i];
                        ghostDirectionRows[i] = 0;
                        ghostDirectionColumns[i] = 0;
                        ghosts[i]->sprite.setPosition({ (ghostHomeColumns[i] + 0.5f) * TILE_SIZE, (ghostHomeRows[i] + 0.5f) * TILE_SIZE });
                    }
                    // edited by abdullahameer
                    // Done by MUHAMMAD KHIZAR REHMAN: reset the release time of ALL 8 ghosts
                    for (int i = 0; i < 8; i++)
                        ghostReleaseTimes[i] = i * 3.f;
                    // done here
                    ghostGameTime = 0;
                    ghostModeTime = 0;
                    ghostScatterMode = false;
                    hasRunStarted = false;
                    resetGhosts = false;
                }

                if (hasRunStarted)
                {
                    float ghostTimeMultiplier = 1.f;
                    if (frenzyIsActive)
                        ghostTimeMultiplier = 2.f;
                    ghostGameTime += deltaTime * ghostTimeMultiplier;   // frenzy: ghosts leave the house twice as fast
                    ghostModeTime += deltaTime;
                }
                if (ghostScatterMode && ghostModeTime >= 7)
                {
                    ghostScatterMode = false; ghostModeTime = 0;
                }        // 7 sec corner run
                else if (!ghostScatterMode && ghostModeTime >= 20)
                {
                    ghostScatterMode = true; ghostModeTime = 0;
                }  // then 20 sec chase

         // pac-man's tile and which way he faces
                int playerRow = int(playerPosition.y) / TILE_SIZE;
                int playerColumn = int(playerPosition.x) / TILE_SIZE;
                int playerDirectionRow = 0, playerDirectionColumn = 0;
                if (playerDirection == 'U')
                    playerDirectionRow = -1;
                else if
                    (playerDirection == 'D') playerDirectionRow = 1;
                else if
                    (playerDirection == 'L') playerDirectionColumn = -1;
                else if
                    (playerDirection == 'R') playerDirectionColumn = 1;

                for (int i = 0; i < activeGhostCount; i++)
                {
                    if (!hasRunStarted || ghostGameTime < ghostReleaseTimes[i] || !ghosts[i]->alive)
                        continue;

                    sf::Vector2f ghostPosition = ghosts[i]->sprite.getPosition();
                    float targetX = (ghostTargetColumns[i] + 0.5f) * TILE_SIZE;
                    float targetY = (ghostTargetRows[i] + 0.5f) * TILE_SIZE;
                    float ghostMovementStep = ghosts[i]->speed * deltaTime * levelSpeedMultiplier;
                    float distanceToTarget = std::abs(targetX - ghostPosition.x) + std::abs(targetY - ghostPosition.y);

                    // still walking to the next tile
                    if (distanceToTarget > ghostMovementStep)
                    {
                        ghostPosition.x += ghostDirectionColumns[i] * ghostMovementStep;
                        ghostPosition.y += ghostDirectionRows[i] * ghostMovementStep;
                        ghosts[i]->sprite.setPosition(ghostPosition);
                        continue;
                    }

                    // reached the tile centre, so snap there and decide the next turn
                    ghosts[i]->sprite.setPosition({ targetX, targetY });
                    ghostRows[i] = ghostTargetRows[i];
                    ghostColumns[i] = ghostTargetColumns[i];
                    // making movements like other ghosts
                    int ghostType = i % 4;      // naye ghosts purane 4 ki tarah chalein
                    // 1) choose the target square
                    int targetRow, targetColumn;
                    bool ghostIsInHouse = (ghostRows[i] >= 9 && ghostRows[i] <= 11 && ghostColumns[i] >= 11 && ghostColumns[i] <= 13);
                    if (ghostIsInHouse)
                    {
                        targetRow = 8; targetColumn = 12;                   // head for the house door
                    }
                    else if (ghosts[i]->isBoss)
                    {
                        targetRow = playerRow; targetColumn = playerColumn;                  // boss always chases pac-man
                    }
                    else if (ghostScatterMode)
                    {
                        targetRow = ghostCornerRows[i]; targetColumn = ghostCornerColumns[i];            // own corner
                    }
                    else if (ghostType == 0)
                    {
                        targetRow = playerRow; targetColumn = playerColumn;                  // red: pac-man himself
                    }
                    else if (ghostType == 1)
                    {
                        targetRow = playerRow + 4 * playerDirectionRow;                  // pink: 4 squares ahead
                        targetColumn = playerColumn + 4 * playerDirectionColumn;
                    }
                    else if (ghostType == 2)
                    {
                        int aheadRow = playerRow + 2 * playerDirectionRow, aheadColumn = playerColumn + 2 * playerDirectionColumn;  // 2 squares ahead
                        targetRow = 2 * aheadRow - ghostRows[0];                     // red -> there, doubled
                        targetColumn = 2 * aheadColumn - ghostColumns[0];
                    }
                    else
                    {
                        int rowDistance = playerRow - ghostRows[i], columnDistance = playerColumn - ghostColumns[i];
                        if (rowDistance * rowDistance + columnDistance * columnDistance < 64)        // closer than 8 squares
                        {
                            targetRow = ghostCornerRows[i]; targetColumn = ghostCornerColumns[i];        // run home
                        }
                        else
                        {
                            targetRow = playerRow; targetColumn = playerColumn;              // far away, so chase
                        }
                    }

                    // 2) of the 4 ways, pick the one that ends closest to the target
                    //    (random: ties are broken randomly)
                    int bestDirection = -1, bestDistance = 999999;
                    int validDirections[4], validDirectionCount = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        if (rowDirections[k] == -ghostDirectionRows[i] && columnDirections[k] == -ghostDirectionColumns[i])
                            continue;                                   // no turning back
                        int nextRow = ghostRows[i] + rowDirections[k], nextColumn = ghostColumns[i] + columnDirections[k];
                        if (nextRow < 0 || nextRow >= MAZE_ROWS || nextColumn < 0 || nextColumn >= MAZE_COLUMNS)
                            continue;
                        if (mazeTiles[nextRow * MAZE_COLUMNS + nextColumn] == 1 || mazeTiles[nextRow * MAZE_COLUMNS + nextColumn] == -1)
                            continue;                                   // wall (or tunnel edge)
                        if (!ghostIsInHouse && nextRow >= 9 && nextRow <= 11 && nextColumn >= 11 && nextColumn <= 13)
                            continue;                                   // can't go back in the house
                        validDirections[validDirectionCount++] = k;
                        int distanceToTargetTile = (nextRow - targetRow) * (nextRow - targetRow) + (nextColumn - targetColumn) * (nextColumn - targetColumn);
                        if (distanceToTargetTile < bestDistance || (distanceToTargetTile == bestDistance && std::rand() % 2 == 0))
                        {
                            bestDistance = distanceToTargetTile;
                            bestDirection = k;
                        }
                    }

                    // random ghost decision: 8% of the time a normal ghost (not the boss,
                    // not leaving the house) takes a random open way at a junction
                    if (validDirectionCount > 1 && !ghostIsInHouse && !ghosts[i]->isBoss && std::rand() % 100 < 8)
                        bestDirection = validDirections[std::rand() % validDirectionCount];

                    if (bestDirection == -1)              // dead end: the only time it may turn back
                    {
                        ghostDirectionRows[i] = -ghostDirectionRows[i];
                        ghostDirectionColumns[i] = -ghostDirectionColumns[i];
                    }
                    else
                    {
                        ghostDirectionRows[i] = rowDirections[bestDirection];
                        ghostDirectionColumns[i] = columnDirections[bestDirection];
                    }
                    ghostTargetRows[i] = ghostRows[i] + ghostDirectionRows[i];
                    ghostTargetColumns[i] = ghostColumns[i] + ghostDirectionColumns[i];
                }
                // boss mode add by abd inwhich when two ghosts are kill so other 2 ghosts turn into boss mode
                // ===== BOSS MODE (5.8) =====
                int aliveCount = 0;
                for (int i = 0; i < activeGhostCount; i++)
                    if (ghosts[i]->alive)
                        aliveCount++;

                if (aliveCount <= 1)                          // only 1 ghost left: it becomes the boss
                {
                    for (int i = 0; i < activeGhostCount; i++)
                    {
                        if (!ghosts[i]->alive || ghosts[i]->isBoss)
                            continue;   // mara hua ya pehle se boss: skip

                        ghosts[i]->isBoss = true;
                        ghosts[i]->speed = 165;                              // 130 x 1.6
                        sf::Texture& bt = *ghostTextures[i];                  // scared texture ho to asli wapas
                        ghosts[i]->sprite.setTexture(bt, true);
                        ghosts[i]->sprite.setScale({ 40.f / bt.getSize().x, 40.f / bt.getSize().y });
                        ghosts[i]->sprite.setOrigin({ bt.getSize().x / 2.f, bt.getSize().y / 2.f });
                        ghosts[i]->sprite.setColor(sf::Color(255, 90, 90));  // laal rang
                    }
                }
                // ===== END BOSS MODE =====



                // ===== LIFE LOSS: ghost touches pac-man =====
                // Done by MUHAMMAD KHIZAR REHMAN
                for (int i = 0; i < activeGhostCount; i++)            // Done by MUHAMMAD KHIZAR REHMAN: all active ghosts (was i < 4)
                {
                    if (!ghosts[i]->alive)
                        continue;                               // Done by MUHAMMAD KHIZAR REHMAN: dead ghosts can't touch
                    sf::Vector2f q = ghosts[i]->sprite.getPosition();
                    float ux = q.x - playerPosition.x, uy = q.y - playerPosition.y;


                    // Edited by abdullahameer for orbit speed
                    if (ux * ux + uy * uy < 22.f * 22.f)        // ghost is touching pac-man
                    {
                        // how many ghosts are still alive on this level
                        int aliveNow = 0;
                        for (int k = 0; k < activeGhostCount; k++)
                            if (ghosts[k]->alive)
                                aliveNow++;

                        if (isGhostKillPowerActive() && !ghosts[i]->isBoss && aliveNow > 1)
                        {
                            // kill-ghost orb: the ghost dies until the next level (a boss ghost can't die)
                            player.score += 200 * scoreMultiplier;
                            ghosts[i]->alive = false;
                            // abd write this line to eat foods
                            unlockGhostAchievement();
                        }
                        else if (!isPlayerShieldActive())
                        {
                            // Stop pellet audio immediately when Pac-Man dies.
                            // The death sound takes priority over every other effect.
                            pelletSound.stop();

                            // Play the death sound only when Pac-Man actually loses a life.
                            for (int soundIndex = 0; soundIndex < 4; soundIndex++)
                                laneWarningSounds[soundIndex]->stop();
                            ghostLaneWasActive = false;

                            // Play the next sound in the game-over playlist.
                            playNextSound(deathSounds, deathSoundLoaded, 3, nextDeathSound);

                            // koi orb active nahi: pehle wala lives wala code (bilkul wese hi)
                            player.lives--;                         // lose one life
                            player.sprite.setPosition({ 54, 846 }); // pac-man goes back to start
                            playerDirection = ' ';
                            requestedDirection = ' ';
                            resetPowerUps(false);                        // orb ke timers band (sirf yeh ek line nayi hai)
                            resetGhosts = true;                              // ghosts go back to the house
                            break;                                   // only one life per hit
                        }
                        // agar invincible chal raha hai to kuch nahi hota
                    }
                }

                // ===== GHOST ENTERS PAC-MAN'S LANE AUDIO WARNING =====
                // Check active, living ghosts after their movement for this frame.
                bool ghostInPacmanLane = false;
                if (hasRunStarted && player.lives > 0)
                {
                    for (int i = 0; i < activeGhostCount; i++)
                    {
                        if (!ghosts[i]->alive || ghostGameTime < ghostReleaseTimes[i])
                            continue;

                        if (isGhostInClearLane(playerPosition, ghosts[i]->sprite.getPosition()))
                        {
                            ghostInPacmanLane = true;
                            break;
                        }
                    }
                }

                // Play once when a ghost enters the lane. It will be allowed
                // to play again only after all ghosts leave Pac-Man's lane.
                if (ghostInPacmanLane && !ghostLaneWasActive)
                {
                    // Chase warning has priority: interrupt pellet-eating audio.
                    pelletSound.stop();

                    // Play the next sound in the chase playlist whenever a
                    // ghost newly enters Pac-Man's unobstructed row or column.
                    playNextSound(laneWarningSounds, laneSoundLoaded, 4, nextLaneSound);
                }
                ghostLaneWasActive = ghostInPacmanLane;
                // ===== END GHOST LANE AUDIO WARNING =====

                // ===== GRAPE: extra life every 5 minutes (5.10) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                if (hasRunStarted)
                    timeSinceGrapeSpawn += deltaTime;
                if (timeSinceGrapeSpawn >= grapeSpawnInterval)         // 5 minutes passed
                {
                    grapeIsVisible = true;          // grape appears
                    timeSinceGrapeSpawn = 0;
                }
                if (grapeIsVisible)
                {
                    float fx = playerPosition.x - 450.f, fy = playerPosition.y - 306.f;
                    if (fx * fx + fy * fy < 20.f * 20.f)     // pac-man touches the grape
                    {
                        player.lives++;                     // extra life
                        grapeIsVisible = false;                          // grape disappears
                    }
                }

                // ===== SPECIAL BONUS ORB (5.2) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                if (!bonusOrbIsVisible)
                {
                    bonusOrbRespawnTimer += deltaTime;               // count the seconds since it was eaten
                    if (bonusOrbRespawnTimer >= bonusOrbRespawnSeconds)          // 2 minutes passed
                    {
                        bonusOrbIsVisible = true;          // the orb is back
                        bonusOrbRespawnTimer = 0;
                    }
                }
                else
                {
                    float ox = playerPosition.x - (bonusOrbColumn + 0.5f) * TILE_SIZE;
                    float oy = playerPosition.y - (bonusOrbRow + 0.5f) * TILE_SIZE;
                    if (ox * ox + oy * oy < 18.f * 18.f)     // pac-man touches the orb
                    {
                        player.score += 1000 * scoreMultiplier;        // 1000 bonus points
                        bonusOrbIsVisible = false;                          // the orb disappears
                        bonusOrbRespawnTimer = 0;                              // the 2 minute countdown starts
                    }
                }
                // ===== END OF GHOST MOVEMENT =====

                // 3. DRAW
            }

            // Move Pac-Man to the other side when he reaches a tunnel edge.
            // working on teleportation

            if (player.sprite.getPosition().y >= 398 && player.sprite.getPosition().y <= 453)
            {
                if (player.sprite.getPosition().x <= 41)
                    player.sprite.setPosition({ 860, 414 });
                else if (player.sprite.getPosition().x >= 882)
                    player.sprite.setPosition({ 43, 414 });
            }

            // top / bottom teleport (the 3-wide gaps in the top and bottom walls, columns 11-13)
            if (player.sprite.getPosition().x >= 396 && player.sprite.getPosition().x < 504)
            {
                if (player.sprite.getPosition().y <= 41)
                    player.sprite.setPosition({ player.sprite.getPosition().x, 860 });
                else if (player.sprite.getPosition().y >= 882)
                    player.sprite.setPosition({ player.sprite.getPosition().x, 43 });
            }


            // Convert the score number to text for the sidebar.
            char buff[12], digits[12];
            int length = 0;
            int n = player.score, d = 0;
            if (n == 0)
            {
                digits[d] = '0';
                d++;
            }
            while (n > 0)
            {
                digits[d] = '0' + n % 10;
                d++;
                n /= 10;
            }
            while (d > 0)
            {
                d--;
                buff[length] = digits[d];
                length++;
            }
            buff[length] = '\0';
            scoreDisplay.setString(buff);

            // Convert the lives number to text for the sidebar.
            char buff2[12], digits2[12];
            int length2 = 0;
            int n2 = player.lives, d2 = 0;
            if (n2 == 0)
            {
                digits2[d2] = '0';
                d2++;
            }
            while (n2 > 0)
            {
                digits2[d2] = '0' + n2 % 10;
                d2++;
                n2 /= 10;
            }
            while (d2 > 0)
            {
                d2--;
                buff2[length2] = digits2[d2];
                length2++;
            }
            buff2[length2] = '\0';
            livesDisplay.setString(buff2);



            // Draw the maze, status panels, pellets, and power-ups for this frame.
            window.clear();
            if (frenzyIsActive)
                mazeBackground.setColor(sf::Color(150, 90, 200));
            else
                mazeBackground.setColor(sf::Color::White);
            window.draw(mazeBackground);
            window.draw(rightSidebar);
            window.draw(gameTitle);
            window.draw(scoreLabel);
            window.draw(scoreDisplay);
            window.draw(livesLabel);
            window.draw(livesDisplay);
            drawPowerUpTimers(window, font);

            // TEST: red squares over wall cells (delete when walls are finished)
            // This is rayan, Dont delete this, i am using it to build maze

            sf::RenderStates glowStates;
            glowStates.blendMode = sf::BlendAdd;   // colors add up, which gives the glow

            // outer glow, then inner glow (additive)
            if (frenzyIsActive)   // frenzy colors: pink / purple
            {
                drawMazeWallLayer(window, 30.f, sf::Color(255, 0, 200, 40), glowStates);
                drawMazeWallLayer(window, 22.f, sf::Color(255, 40, 220, 90), glowStates);
            }
            else
            {
                drawMazeWallLayer(window, 30.f, sf::Color(255, 180, 0, 40), glowStates);
                drawMazeWallLayer(window, 22.f, sf::Color(255, 200, 20, 90), glowStates);
            }

            // bright thin core on top (normal drawing)
            sf::Color wallColor(255, 248, 190);
            if (frenzyIsActive)
                wallColor = sf::Color(255, 200, 255);
            drawMazeWallLayer(window, 10.f, wallColor, sf::RenderStates::Default);
            //draw dots
            sf::CircleShape dot(3.f);
            dot.setFillColor(sf::Color::Yellow);

            for (int r = 0; r < MAZE_ROWS; r++)
            {
                for (int c = 0; c < MAZE_COLUMNS; c++)
                {
                    if (mazeTiles[r * MAZE_COLUMNS + c] == 2)
                    {
                        dot.setPosition({ (c + 0.5f) * TILE_SIZE - 3.f, (r + 0.5f) * TILE_SIZE - 3.f });
                        window.draw(dot);
                    }
                }
            }

            // ===== GATES + SIDEBAR INFO (5.6) + GRAPE DRAW (5.10) =====
            // Done by MUHAMMAD KHIZAR REHMAN
            gateShape.setPosition({ float(entryGateColumn * TILE_SIZE), float(entryGateRow * TILE_SIZE) });
            gateShape.setFillColor(sf::Color(0, 150, 255, 140));          // entry gate: blue
            window.draw(gateShape);

            gateShape.setPosition({ float(exitGateColumn * TILE_SIZE), float(exitGateRow * TILE_SIZE) });
            if (pelletsCollected >= totalPellets)
                gateShape.setFillColor(sf::Color(50, 255, 80, 160));      // exit open: green
            else
                gateShape.setFillColor(sf::Color(255, 50, 50, 140));      // exit locked: red
            window.draw(gateShape);

            char gateInfo[96];
            char gateState[8] = "LOCKED";
            if (pelletsCollected >= totalPellets)
                std::snprintf(gateState, sizeof(gateState), "OPEN");
            std::snprintf(gateInfo, sizeof(gateInfo),
                "Level: %d\nPellets: %d/%d\nEXIT: %s",
                currentLevel, pelletsCollected, totalPellets, gateState);
            gateInfoText.setString(gateInfo);
            window.draw(gateInfoText);

            if (grapeIsVisible)
                window.draw(grapeSprite);

            if (bonusOrbIsVisible)
                window.draw(bonusOrbShape);   // Done by MUHAMMAD KHIZAR REHMAN: special bonus orb (5.2)

            // abdullahameer code here

            drawPowerUps(window, runTimeSeconds);
            window.draw(leftSidebar);
            drawAchievementBoard(window, font);
            drawProgressLeaderboard(window, font);

            if (frenzyIsActive)
            {
                char frenzyText[32];
                std::snprintf(frenzyText, sizeof(frenzyText), "FRENZY x5: %d", int(frenzyTimeLeft) + 1);
                sf::Text frenzyLabel(font, frenzyText, 30);
                frenzyLabel.setFillColor(sf::Color::Magenta);
                frenzyLabel.setPosition({ 930.f, 560.f });
                window.draw(frenzyLabel);
            }
        }

        window.draw(player.sprite);   // last, so he is on top
        if (isGamePaused)
        {
            window.draw(overlay);
            window.draw(paused_text);
            window.draw(paused_text_additional);
        }


        for (int i = 0; i < activeGhostCount; i++)
            if (ghosts[i]->alive) window.draw(ghosts[i]->sprite);

        if (levelBannerTimer > 0)                                    // LEVEL n screen
        {
            char levelText[32];
            std::snprintf(levelText, sizeof(levelText), "LEVEL %d", currentLevel);
            sf::Text lvlText(font, levelText, 120);
            lvlText.setFillColor(sf::Color::Yellow);
            lvlText.setPosition({ 250.f, 380.f });
            window.draw(lvlText);
        }


        // Show the game-over screen and restart the game when requested.
        if (player.lives <= 0)
        {
            // save the score if not already done (leaderboard code by hmza):
            if (!scoreWasSaved)
            {
                addScoreToLeaderboard(playerName, player.score, int(runTimeSeconds));
                scoreWasSaved = true;
            }

            window.draw(game_over_screen);
            window.draw(game_over_text);
            window.draw(game_over_text_additional);
            drawScoreLeaderboard(window, font);              // shows the top 5 list with names (leaderboard code by hmza)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                player.lives = 1;
                player.score = -10;
                placeMazeDots();
                player.sprite.setPosition({ 54, 846 });
                clock.restart();
                playerDirection = ' ';
                requestedDirection = ' ';
                runTimeSeconds = 0;
                // new run starts from zero (leaderboard code by hmza)
                // edit by abd
                levelStartTime = 0;
                scoreWasSaved = false;
                frenzyIsActive = false;      // frenzy resets on restart (hmza)
                timeUntilFrenzy = 0;
                // allow the next game over to save again
                 // abdullahameer code
                resetPowerUps(true);

                // Done by MUHAMMAD KHIZAR REHMAN: gates and grape start fresh
                pelletsCollected = 0;
                for (int k = 0; k < MAZE_COLUMNS * MAZE_ROWS; k++)
                    pelletRespawnTimers[k] = 0;
                grapeIsVisible = false;
                timeSinceGrapeSpawn = 0;
                bonusOrbIsVisible = true;             // Done by MUHAMMAD KHIZAR REHMAN: bonus orb starts fresh
                bonusOrbRespawnTimer = 0;

                // ===== LEVEL aur GHOSTS reset (abdullahameer) =====
                currentLevel = 1;
                levelSpeedMultiplier = 1.f;
                activeGhostCount = 4;
                levelBannerTimer = 0;

                for (int i = 0; i < 8; i++)
                {
                    ghosts[i]->alive = true;
                    ghosts[i]->isBoss = false;
                    ghosts[i]->speed = 130;
                    ghosts[i]->sprite.setColor(sf::Color::White);
                    ghosts[i]->sprite.setTexture(*ghostTextures[i], true);
                    ghosts[i]->sprite.setScale({ ghostSizes[i] / ghostTextures[i]->getSize().x, ghostSizes[i] / ghostTextures[i]->getSize().y });
                    ghosts[i]->sprite.setOrigin({ ghostTextures[i]->getSize().x / 2.f, ghostTextures[i]->getSize().y / 2.f });
                }
                ghostsScared = false;
                resetGhosts = true;                    // ghosts wapas house mein
                // ===== END RESET =====
            }


        }
        window.display();

    }
}

// Fills walkable maze tiles with pellets, leaving the ghost house empty.
void placeMazeDots()
{
    for (int r = 0; r < MAZE_ROWS; r++)
    {
        for (int c = 0; c < MAZE_COLUMNS; c++)
        {
            if (mazeTiles[r * MAZE_COLUMNS + c] != 0)
                continue;

            // no dots inside the ghost house
            bool inHouse = (r >= 9 && r <= 12 && c >= 10 && c <= 14);
            if (!inHouse)
                mazeTiles[r * MAZE_COLUMNS + c] = 2;
        }
    }
}

// Draws wall tiles and joins nearby tiles into a smooth glowing layer.
void drawMazeWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states)
{
    sf::RectangleShape piece;
    piece.setFillColor(color);

    float margin = (TILE_SIZE - thickness) / 2.f;   // gap between tile edge and the shape

    for (int row = 0; row < MAZE_ROWS; row++)
    {
        for (int col = 0; col < MAZE_COLUMNS; col++)
        {
            if (!isWallTile(row, col))
                continue;

            float x = float(col * TILE_SIZE);
            float y = float(row * TILE_SIZE);

            // square in the middle of the tile
            piece.setSize({ thickness, thickness });
            piece.setPosition({ x + margin, y + margin });
            window.draw(piece, states);

            // connector to the wall on the right
            if (isWallTile(row, col + 1))
            {
                piece.setSize({ TILE_SIZE - thickness, thickness });
                piece.setPosition({ x + margin + thickness, y + margin });
                window.draw(piece, states);
            }

            // connector to the wall below
            if (isWallTile(row + 1, col))
            {
                piece.setSize({ thickness, TILE_SIZE - thickness });
                piece.setPosition({ x + margin, y + margin + thickness });
                window.draw(piece, states);
            }
        }
    }
}
