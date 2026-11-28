#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <optional> // Abd
#include <iostream>
#include <cmath>   // Done by MUHAMMAD KHIZAR REHMAN
#include <cstdlib> // rand / srand (random ghost decisions, random orb spawning)
#include <ctime>   // time stamps on leaderboard runs, seed for rand

const int PLAYER_NAME_CAPACITY = 11;
const int PLAYER_INPUT_LENGTH = 10;

const char laneSoundFiles[4][40] = {
    "pakad-mc.mp3", "aree-cid.mp3",
    "cid-maiya-chood.mp3", "cid-aak-bar-mil-jabeta.mp3"
};
const char deathSoundFiles[3][40] = {
    "ghost3.mp3.mpeg", "cid-acp-pradhuman-fasi.mp3",
    "cid-ke-bakchodi.mp3"
};

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

void placeMazeDots();
void drawMazeWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states);

// making constants coz these are fixed mapping values of game.
const int TILE_SIZE = 36;   // size of one square, in pixels
const int MAZE_COLUMN_COUNT = 25;   // how many squares across
const int MAZE_ROW_COUNT = 25;   // how many squares down


// 0 for path with point, 1 for wall, -1 for path without point
int mazeTiles[MAZE_COLUMN_COUNT * MAZE_ROW_COUNT] = {
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
const int   POWER_UP_ORB_COUNT = 4;
const int   POWER_UP_TILE_SIZE = 36;
const int  powerUpStartRows[POWER_UP_ORB_COUNT] = { 1, 1, 13, 23 };   // where each orb starts every level
const int  powerUpStartColumns[POWER_UP_ORB_COUNT] = { 1, 23, 12, 23 };
int  powerUpRows[POWER_UP_ORB_COUNT] = { 1, 1, 13, 23 };
int  powerUpColumns[POWER_UP_ORB_COUNT] = { 1, 23, 12, 23 };
int  powerUpTypes[POWER_UP_ORB_COUNT] = { 1, 2, 3, 1 };    // 1 = speed, 2 = invincible, 3 = kill-ghost
bool powerUpIsVisible[POWER_UP_ORB_COUNT] = { true, true, true, true };
const float POWER_UP_RESPAWN_SECONDS = 60.f;
float powerUpRespawnTimers[POWER_UP_ORB_COUNT] = { 0.f, 0.f, 0.f, 0.f };

const float SPEED_POWER_SECONDS = 10.f;
const float SHIELD_POWER_SECONDS = 10.f;
const float GHOST_KILL_POWER_SECONDS = 10.f;
const float SPEED_POWER_MULTIPLIER = 1.6f;

float speedPowerTimer = 0, shieldPowerTimer = 0, ghostKillPowerTimer = 0;
// working for next levels by abd
int   level = 1;               // abhi kaun sa level hai
float levelSpeedMultiplier = 1.f;        // har level par x1.10
int   activeGhostCount = 4;          // abhi kitne ghosts khel mein hain
const int MAX_GHOST_COUNT = 8;
float levelBannerTimer = 0;         // "LEVEL 2" screen ka timer
sf::Color getPowerUpColor(int type)
{
    if (type == 1)
        return sf::Color(0, 220, 255);
    if (type == 2)
        return sf::Color(255, 215, 0);
    return sf::Color(255, 40, 40);
}

// random orb spawning: moves orb o to a random open tile (not in the ghost house,
// not on the gates, bonus orb, grape or another orb)


// this one used to pick a random open path
void placePowerUpOnRandomTile(int o)
{
    for (int tries = 0; tries < 200; tries++)
    {
        int r = std::rand() % MAZE_ROW_COUNT;
        int c = std::rand() % MAZE_COLUMN_COUNT;

        if (mazeTiles[r * MAZE_COLUMN_COUNT + c] != 0 && mazeTiles[r * MAZE_COLUMN_COUNT + c] != 2)
            continue;                                   // wall or tunnel edge
        if (r >= 9 && r <= 12 && c >= 10 && c <= 14)
            continue;                                   // ghost house
        if (r == 8 && (c == 11 || c == 12))
            continue;                                   // bonus orb + grape tiles
        if ((r == 23 && c == 1) || (r == 1 && c == 23))
            continue;                                   // entry / exit gate

        bool taken = false;
        for (int k = 0; k < POWER_UP_ORB_COUNT; k++)
            if (k != o && powerUpIsVisible[k] && powerUpRows[k] == r && powerUpColumns[k] == c)
                taken = true;
        if (taken)
            continue;

        powerUpRows[o] = r;
        powerUpColumns[o] = c;
        return;
    }
}

void updatePowerUps(float deltaTime)
{
    if (speedPowerTimer > 0)
        speedPowerTimer = std::max(0.f, speedPowerTimer - deltaTime);
    if (shieldPowerTimer > 0)
        shieldPowerTimer = std::max(0.f, shieldPowerTimer - deltaTime);
    if (ghostKillPowerTimer > 0)
        ghostKillPowerTimer = std::max(0.f, ghostKillPowerTimer - deltaTime);

    // Each collected power-up orb respawns 60 seconds after it was collected,
    // on a random tile.
    for (int o = 0; o < POWER_UP_ORB_COUNT; o++)
    {
        if (!powerUpIsVisible[o] && powerUpRespawnTimers[o] > 0.f)
        {
            powerUpRespawnTimers[o] -= deltaTime;
            if (powerUpRespawnTimers[o] <= 0.f)
            {
                powerUpRespawnTimers[o] = 0.f;
                placePowerUpOnRandomTile(o);
                powerUpIsVisible[o] = true;
            }
        }
    }
}
// used for functionality of increasing orb speed 
float getPlayerSpeedMultiplier()
{
    return (speedPowerTimer > 0 ? SPEED_POWER_MULTIPLIER : 1.f) * levelSpeedMultiplier;
}

bool  isPlayerShieldActive()
{
    return shieldPowerTimer > 0;
}
bool  isGhostKillPowerActive()
{
    return ghostKillPowerTimer > 0;
}


// calculates distance between pac man and orb and if less than 18 then treats as picked by hiding and starting its timer
int collectPowerUps(sf::Vector2f playerPosition)
{
    int bonus = 0;
    for (int o = 0; o < POWER_UP_ORB_COUNT; o++)
    {
        if (!powerUpIsVisible[o]) continue;
        float dx = playerPosition.x - (powerUpColumns[o] + 0.5f) * POWER_UP_TILE_SIZE;
        float dy = playerPosition.y - (powerUpRows[o] + 0.5f) * POWER_UP_TILE_SIZE;
        if (dx * dx + dy * dy < 18.f * 18.f)
        {
            powerUpIsVisible[o] = false;
            powerUpRespawnTimers[o] = POWER_UP_RESPAWN_SECONDS;
            if (powerUpTypes[o] == 1) speedPowerTimer = SPEED_POWER_SECONDS;
            else if (powerUpTypes[o] == 2) shieldPowerTimer = SHIELD_POWER_SECONDS;
            else ghostKillPowerTimer = GHOST_KILL_POWER_SECONDS;
            bonus += 50;
        }
    }
    return bonus;
}

// resets an orb
void resetPowerUps(bool respawnOrbs)
{
    speedPowerTimer = 0;
    shieldPowerTimer = 0;
    ghostKillPowerTimer = 0;
    if (respawnOrbs)
    {
        for (int o = 0; o < POWER_UP_ORB_COUNT; o++)
        {
            powerUpIsVisible[o] = true;
            powerUpRespawnTimers[o] = 0.f;
            powerUpRows[o] = powerUpStartRows[o];      // back to the starting tile
            powerUpColumns[o] = powerUpStartColumns[o];
        }
    }
}


// draws an orb on window
void drawPowerUps(sf::RenderWindow& window, float t)
{
    sf::CircleShape shape;
    for (int o = 0; o < POWER_UP_ORB_COUNT; o++)
    {
        if (!powerUpIsVisible[o]) continue;
        float rad = 9.f + 2.f * std::sin(t * 6.f);
        shape.setRadius(rad);
        shape.setOrigin({ rad, rad });
        shape.setPosition({ (powerUpColumns[o] + 0.5f) * POWER_UP_TILE_SIZE, (powerUpRows[o] + 0.5f) * POWER_UP_TILE_SIZE });
        shape.setFillColor(getPowerUpColor(powerUpTypes[o]));
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
    }
}

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
    sf::Text hudText(font, text, 30);
    hudText.setFillColor(sf::Color::Red);
    hudText.setPosition({ 930.f, 430.f });
    window.draw(hudText);
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

const char achievementNames[ACHIEVEMENT_COUNT][20] = { "SCORE 500", "SCORE 1500", "SCORE 3000", "GHOST HUNTER", "SPEED RUNNER" };
bool  achievementIsUnlocked[ACHIEVEMENT_COUNT] = { false, false, false, false, false };
float achievementPopupTimer = 0;
char achievementPopupText[64] = "";
char currentPlayerName[PLAYER_NAME_CAPACITY] = "PLAYER";
float levelStartTime = 0;
// abdullahameer added achievemts board
void updateProgressLeaderboard(const char name[], int count);   // forward declaration

int countUnlockedAchievements()
{
    int c = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        if (achievementIsUnlocked[i]) c++;
    return c;
}
void saveAchievements()
{
    char filename[20];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", currentPlayerName);
    std::ofstream f(filename);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        f << (achievementIsUnlocked[i] ? 1 : 0) << " ";
}

void loadPlayerAchievements(const char name[])
{
    std::snprintf(currentPlayerName, sizeof(currentPlayerName), "%s", name);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        achievementIsUnlocked[i] = false;
    char filename[20];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", currentPlayerName);
    std::ifstream f(filename);
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        int v = 0;
        if (f >> v) achievementIsUnlocked[i] = (v == 1);
    }
}

void unlockAchievement(int i)
{
    if (achievementIsUnlocked[i]) return;
    achievementIsUnlocked[i] = true;
    std::snprintf(achievementPopupText, sizeof(achievementPopupText), "UNLOCKED: %s", achievementNames[i]);
    achievementPopupTimer = 3.f;
    saveAchievements();
    updateProgressLeaderboard(currentPlayerName, 0);
}

void checkScoreAchievements(int score)
{
    if (score >= 500)  unlockAchievement(0);
    if (score >= 1500) unlockAchievement(1);
    if (score >= 3000) unlockAchievement(2);
}

void unlockGhostAchievement() { unlockAchievement(3); }

void checkFastLevelAchievement(float seconds)
{
    if (seconds <= FAST_LEVEL_TIME_LIMIT) unlockAchievement(4);
}

void updateAchievementPopup(float deltaTime)
{
    if (achievementPopupTimer > 0) achievementPopupTimer -= deltaTime;
}

void drawOldAchievementBoard(sf::RenderWindow& window, const sf::Font& font)
{
    char playerText[20];
    std::snprintf(playerText, sizeof(playerText), "PLAYER: %s", currentPlayerName);
    sf::Text who(font, playerText, 22);
    who.setFillColor(sf::Color::White);
    who.setPosition({ ACHIEVEMENT_PANEL_X, ACHIEVEMENT_PANEL_Y });
    window.draw(who);

    sf::Text title(font, "ACHIEVEMENTS", 24);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({ ACHIEVEMENT_PANEL_X, ACHIEVEMENT_PANEL_Y + 26.f });
    window.draw(title);

    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        char line[32];
        std::snprintf(line, sizeof(line), "%s%s",
            achievementIsUnlocked[i] ? "[X] " : "[ ] ", achievementNames[i]);
        sf::Text t(font, line, 20);
        t.setFillColor(achievementIsUnlocked[i] ? sf::Color(255, 215, 0) : sf::Color(160, 160, 160));
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
char leaderboardPlayerNames[MAX_LEADERBOARD_ENTRIES][PLAYER_NAME_CAPACITY];
int leaderboardLevelsCleared[MAX_LEADERBOARD_ENTRIES];   // levels cleared (0..TOTAL_LEVEL_COUNT)
int leaderboardAchievementsUnlocked[MAX_LEADERBOARD_ENTRIES];      // achievements unlocked (0..ACHIEVEMENT_COUNT)
int leaderboardEntryCount = 0;

int getLeaderboardPlayerTotal(int i) { return leaderboardLevelsCleared[i] + leaderboardAchievementsUnlocked[i]; }

// counts unlocked achievements from a player's own file
int countPlayerAchievements(const char name[])
{
    char filename[20];
    std::snprintf(filename, sizeof(filename), "ach_%s.txt", name);
    std::ifstream a(filename);
    int c = 0, v = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT && (a >> v); i++)
        if (v == 1) c++;
    return c;
}

void saveProgressLeaderboard()
{
    std::ofstream f("top_progress.txt");
    for (int i = 0; i < leaderboardEntryCount; i++)
        f << leaderboardPlayerNames[i] << " " << leaderboardLevelsCleared[i] << "\n";
}

void sortProgressLeaderboard()
{
    for (int i = 0; i < leaderboardEntryCount - 1; i++)
    {
        for (int j = 0; j < leaderboardEntryCount - 1 - i; j++)
        {
            bool swapNow = getLeaderboardPlayerTotal(j) < getLeaderboardPlayerTotal(j + 1) ||
                (getLeaderboardPlayerTotal(j) == getLeaderboardPlayerTotal(j + 1) && leaderboardAchievementsUnlocked[j] < leaderboardAchievementsUnlocked[j + 1]);
            if (swapNow)
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

void updateProgressLeaderboard(const char name[], int levels)
{
    if (levels > TOTAL_LEVEL_COUNT)
        levels = TOTAL_LEVEL_COUNT;

    int idx = -1;
    for (int i = 0; i < leaderboardEntryCount; i++)
        if (std::strcmp(leaderboardPlayerNames[i], name) == 0) { idx = i; break; }

    if (idx == -1)
    {
        if (leaderboardEntryCount >= MAX_LEADERBOARD_ENTRIES) return;
        idx = leaderboardEntryCount++;
        std::snprintf(leaderboardPlayerNames[idx], sizeof(leaderboardPlayerNames[idx]), "%s", name);
        leaderboardLevelsCleared[idx] = 0;
    }
    if (levels > leaderboardLevelsCleared[idx])
        leaderboardLevelsCleared[idx] = levels;
    leaderboardAchievementsUnlocked[idx] = countPlayerAchievements(name);

    sortProgressLeaderboard();
    saveProgressLeaderboard();
}

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
        t.setFillColor(i == 0 ? sf::Color(255, 215, 0) : sf::Color::White);
        t.setPosition({ LEADERBOARD_PANEL_X, LEADERBOARD_PANEL_Y + 26.f + i * 24.f });
        window.draw(t);
    }
}




// better presentable form by abdullahameer
// ===== PRETTY BOARDS (presentable look) =====
void drawPanelText(sf::RenderWindow& w, const sf::Font& font, const char s[], unsigned int size, sf::Color col, float x, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    t.setPosition({ x, y });
    w.draw(t);
}

void drawCenteredPanelText(sf::RenderWindow& w, const sf::Font& font, const char s[], unsigned int size, sf::Color col, float centerX, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, 0.f });
    t.setPosition({ centerX, y });
    w.draw(t);
}

void drawPanelBox(sf::RenderWindow& w, float x, float y, float width, float height)
{
    sf::RectangleShape r({ width, height });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(0, 0, 0, 190));
    r.setOutlineThickness(2.f);
    r.setOutlineColor(sf::Color(255, 215, 0));
    w.draw(r);
}

void drawPanelLine(sf::RenderWindow& w, float x, float y, float width)
{
    sf::RectangleShape r({ width, 2.f });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(255, 215, 0, 160));
    w.draw(r);
}


void drawProgressLeaderboard(sf::RenderWindow& window, const sf::Font& font)
{
    const float px = -290.f, pw = 280.f, top = 40.f;
    const int MAXPTS = TOTAL_LEVEL_COUNT + ACHIEVEMENT_COUNT;
    sf::Color medal[3] = { sf::Color(255, 215, 0), sf::Color(200, 200, 215), sf::Color(205, 127, 50) };

    drawPanelBox(window, px, top, pw, 200.f);
    drawCenteredPanelText(window, font, "TOP 3 PLAYERS", 26, sf::Color::Yellow, px + pw / 2.f, top + 8.f);
    drawPanelLine(window, px + 10.f, top + 44.f, pw - 20.f);

    for (int i = 0; i < 3; i++)
    {
        float ry = top + 56.f + i * 46.f;
        bool has = (i < leaderboardEntryCount);

        sf::CircleShape badge(13.f);
        badge.setPosition({ px + 12.f, ry + 4.f });
        badge.setFillColor(has ? medal[i] : sf::Color(70, 70, 70));
        window.draw(badge);
        char rank[16];
        std::snprintf(rank, sizeof(rank), "%d", i + 1);
        drawCenteredPanelText(window, font, rank, 18, sf::Color::Black, px + 25.f, ry + 6.f);

        if (!has)
        {
            drawPanelText(window, font, "---", 22, sf::Color(120, 120, 120), px + 48.f, ry + 4.f);
            continue;
        }

        drawPanelText(window, font, leaderboardPlayerNames[i], 22, i == 0 ? sf::Color(255, 215, 0) : sf::Color::White, px + 48.f, ry);

        for (int s = 0; s < MAXPTS; s++)
        {
            sf::RectangleShape seg({ 13.f, 8.f });
            seg.setPosition({ px + 48.f + s * 16.f, ry + 30.f });
            seg.setFillColor(s < getLeaderboardPlayerTotal(i) ? medal[i] : sf::Color(70, 70, 70));
            window.draw(seg);
        }

        char progress[24];
        std::snprintf(progress, sizeof(progress), "%d/%d", getLeaderboardPlayerTotal(i), MAXPTS);
        drawPanelText(window, font, progress,
            20, sf::Color::White, px + pw - 64.f, ry + 2.f);
        char details[24];
        std::snprintf(details, sizeof(details), "L%d A%d", leaderboardLevelsCleared[i], leaderboardAchievementsUnlocked[i]);
        drawPanelText(window, font, details,
            14, sf::Color(200, 200, 200), px + pw - 62.f, ry + 26.f);
    }
}


void drawAchievementBoard(sf::RenderWindow& window, const sf::Font& font)
{
    const float px = -290.f, pw = 280.f, top = 270.f;
    const char achDesc[ACHIEVEMENT_COUNT][32] = { "Reach 500 points", "Reach 1500 points", "Reach 3000 points",
                                          "Eat a ghost (red orb)", "Clear a level < 2 min" };

    int unlocked = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
        if (achievementIsUnlocked[i]) unlocked++;

    drawPanelBox(window, px, top, pw, 330.f);
    drawCenteredPanelText(window, font, "ACHIEVEMENTS", 26, sf::Color::Yellow, px + pw / 2.f, top + 8.f);
    drawPanelLine(window, px + 10.f, top + 44.f, pw - 20.f);

    char playerText[20];
    std::snprintf(playerText, sizeof(playerText), "PLAYER: %s", currentPlayerName);
    drawPanelText(window, font, playerText, 18, sf::Color::White, px + 12.f, top + 52.f);
    char progress[16];
    std::snprintf(progress, sizeof(progress), "%d/%d", unlocked, ACHIEVEMENT_COUNT);
    drawPanelText(window, font, progress,
        18, sf::Color(255, 215, 0), px + pw - 50.f, top + 52.f);

    sf::RectangleShape barBack({ pw - 24.f, 8.f });
    barBack.setPosition({ px + 12.f, top + 80.f });
    barBack.setFillColor(sf::Color(70, 70, 70));
    window.draw(barBack);
    sf::RectangleShape barFill({ (pw - 24.f) * unlocked / ACHIEVEMENT_COUNT, 8.f });
    barFill.setPosition({ px + 12.f, top + 80.f });
    barFill.setFillColor(sf::Color(255, 215, 0));
    window.draw(barFill);

    for (int i = 0; i < ACHIEVEMENT_COUNT; i++)
    {
        float ry = top + 100.f + i * 44.f;
        bool done = achievementIsUnlocked[i];

        sf::RectangleShape check({ 20.f, 20.f });
        check.setPosition({ px + 14.f, ry + 4.f });
        check.setFillColor(done ? sf::Color(255, 215, 0) : sf::Color(0, 0, 0, 0));
        check.setOutlineThickness(2.f);
        check.setOutlineColor(done ? sf::Color(255, 215, 0) : sf::Color(150, 150, 150));
        window.draw(check);
        if (done)
            drawCenteredPanelText(window, font, "X", 16, sf::Color::Black, px + 24.f, ry + 5.f);

        drawPanelText(window, font, achievementNames[i], 20,
            done ? sf::Color(255, 215, 0) : sf::Color(170, 170, 170), px + 44.f, ry);
        drawPanelText(window, font, achDesc[i], 14,
            done ? sf::Color(220, 220, 220) : sf::Color(120, 120, 120), px + 44.f, ry + 24.f);
    }

    if (achievementPopupTimer > 0)   // "UNLOCKED" popup mazeTiles ke upar
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

// is wall 1 is used in making sure player cant cross walls
bool isWallAtPosition(float x, float y)
{
    if (x < 0 || y < 0)
        return true;

    int col = int(x) / TILE_SIZE;
    int row = int(y) / TILE_SIZE;

    if (col >= MAZE_COLUMN_COUNT || row >= MAZE_ROW_COUNT)
        return true;

    return mazeTiles[row * MAZE_COLUMN_COUNT + col] == 1;
}

// is wall2 is used in map construction
bool isWallTile(int row, int col)
{
    if (row < 0 || row >= MAZE_ROW_COUNT || col < 0 || col >= MAZE_COLUMN_COUNT)
        return false;
    return mazeTiles[row * MAZE_COLUMN_COUNT + col] == 1;
}

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

// Returns true when Pac-Man and a ghost are aligned horizontally or
// vertically and no wall tile blocks the line between them.
bool isGhostInClearLane(sf::Vector2f pacmanPos, sf::Vector2f ghostPos)
{
    const float alignmentTolerance = 3.f;

    if (pacmanPos.x < 0.f || pacmanPos.y < 0.f ||
        ghostPos.x < 0.f || ghostPos.y < 0.f ||
        pacmanPos.x >= MAZE_COLUMN_COUNT * TILE_SIZE || ghostPos.x >= MAZE_COLUMN_COUNT * TILE_SIZE ||
        pacmanPos.y >= MAZE_ROW_COUNT * TILE_SIZE || ghostPos.y >= MAZE_ROW_COUNT * TILE_SIZE)
        return false;

    int pacRow = (int)pacmanPos.y / TILE_SIZE;
    int pacCol = (int)pacmanPos.x / TILE_SIZE;
    int ghostRow = (int)ghostPos.y / TILE_SIZE;
    int ghostCol = (int)ghostPos.x / TILE_SIZE;

    // Same horizontal lane.
    if (pacRow == ghostRow && std::abs(pacmanPos.y - ghostPos.y) <= alignmentTolerance)
    {
        int firstCol = std::min(pacCol, ghostCol) + 1;
        int lastCol = std::max(pacCol, ghostCol);

        for (int col = firstCol; col < lastCol; col++)
        {
            if (mazeTiles[pacRow * MAZE_COLUMN_COUNT + col] == 1)
                return false;
        }
        return true;
    }

    // Same vertical lane.
    if (pacCol == ghostCol && std::abs(pacmanPos.x - ghostPos.x) <= alignmentTolerance)
    {
        int firstRow = std::min(pacRow, ghostRow) + 1;
        int lastRow = std::max(pacRow, ghostRow);

        for (int row = firstRow; row < lastRow; row++)
        {
            if (mazeTiles[row * MAZE_COLUMN_COUNT + pacCol] == 1)
                return false;
        }
        return true;
    }

    return false;
}
//leaderboard code by hmza:

char scorePlayerNames[6][PLAYER_NAME_CAPACITY];    // player name of each run
int scoreValues[6];           // 5 bestDirectionIndex scores + 1 spare slot
int scoreDurations[6];            // seconds each run lasted
std::time_t scoreTimestamps[6];   // when each run ended (unix time)
int savedScoreCount = 0;         // how many scores we have right now

// turns a unix time into text like "24/11 14:30"
void formatScoreDateAndTime(std::time_t t, char buffer[])
{
    std::tm tmv = {};
#ifdef _WIN32
    localtime_s(&tmv, &t);
#else
    localtime_r(&t, &tmv);
#endif
    std::strftime(buffer, 32, "%d/%m %H:%M", &tmv);
}

// read the file when the game starts (new file: it also stores name + time stamp)
void loadScoreLeaderboard()
{
    std::ifstream file("leaderboard_v2.txt");
    savedScoreCount = 0;
    long long stamp = 0;
    while (savedScoreCount < 5)
    {
        file.width(PLAYER_NAME_CAPACITY);
        if (!(file >> scorePlayerNames[savedScoreCount] >> scoreValues[savedScoreCount]
            >> scoreDurations[savedScoreCount] >> stamp))
            break;
        scoreTimestamps[savedScoreCount] = (std::time_t)stamp;
        savedScoreCount++;
    }
}

// add a new score, sort, keep 5, save
void addScoreToLeaderboard(const char name[], int score, int seconds)
{
    std::snprintf(scorePlayerNames[savedScoreCount], sizeof(scorePlayerNames[savedScoreCount]), "%s", name);
    scoreValues[savedScoreCount] = score;
    scoreDurations[savedScoreCount] = seconds;
    scoreTimestamps[savedScoreCount] = std::time(nullptr);   // time stamp of this run
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

                temp = scoreDurations[j];
                scoreDurations[j] = scoreDurations[j + 1];
                scoreDurations[j + 1] = temp;

                char name[PLAYER_NAME_CAPACITY];
                std::snprintf(name, sizeof(name), "%s", scorePlayerNames[j]);
                std::snprintf(scorePlayerNames[j], sizeof(scorePlayerNames[j]), "%s", scorePlayerNames[j + 1]);
                std::snprintf(scorePlayerNames[j + 1], sizeof(scorePlayerNames[j + 1]), "%s", name);

                std::time_t ts = scoreTimestamps[j];
                scoreTimestamps[j] = scoreTimestamps[j + 1];
                scoreTimestamps[j + 1] = ts;
            }
        }
    }

    if (savedScoreCount > 5)
        savedScoreCount = 5;                  // forget the 6th

    // save to the file
    std::ofstream file("leaderboard_v2.txt");
    for (int i = 0; i < savedScoreCount; i++)
        file << scorePlayerNames[i] << " " << scoreValues[i] << " " << scoreDurations[i] << " "
        << (long long)scoreTimestamps[i] << "\n";
}

// show the list on the screen
void drawScoreLeaderboard(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title(font, "TOP 5 SCORES", 40);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin({ tb.position.x + tb.size.x / 2.f, 0.f });   // horizontal centre
    title.setPosition({ 600.f, 460.f });
    window.draw(title);

    // one row of text: rank, player name, score, run time, date + time of the run
    // find the widest row so the whole list is centred as one block
    float maxW = 0.f;
    for (int i = 0; i < savedScoreCount; i++)
    {
        char line[128];
        char stamp[32];
        formatScoreDateAndTime(scoreTimestamps[i], stamp);
        std::snprintf(line, sizeof(line), "%d.  %s  %d pts  %d sec  %s",
            i + 1, scorePlayerNames[i], scoreValues[i], scoreDurations[i], stamp);
        sf::Text row(font, line, 26);
        float w = row.getLocalBounds().size.x;
        if (w > maxW)
            maxW = w;
    }

    for (int i = 0; i < savedScoreCount; i++)
    {
        char line[128];
        char stamp[32];
        formatScoreDateAndTime(scoreTimestamps[i], stamp);
        std::snprintf(line, sizeof(line), "%d.  %s  %d pts  %d sec  %s",
            i + 1, scorePlayerNames[i], scoreValues[i], scoreDurations[i], stamp);
        sf::Text row(font, line, 26);
        row.setPosition({ 600.f - maxW / 2.f, 520.f + i * 40.f });
        window.draw(row);
    }
}

// Plays the next sound that loaded successfully.
void playNextAvailableSound(sf::Sound* sounds[], const bool loaded[],
    int soundCount, int& nextIndex)
{
    for (int attempt = 0; attempt < soundCount; attempt++)
    {
        int index = nextIndex;
        nextIndex = (nextIndex + 1) % soundCount;

        if (!loaded[index])
            continue;

        for (int i = 0; i < soundCount; i++)
            sounds[i]->stop();

        sounds[index]->play();
        return;
    }
}

// Checks whether the menu music or another game sound is playing.
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

int main()
{
    // seed the random numbers (ghost decisions, orb spawning)
    std::srand((unsigned int)std::time(nullptr));

    // window
    sf::RenderWindow window(sf::VideoMode({ 1500, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);
    // added by abd for distanceToTarget achievement board
    sf::View gameView(sf::FloatRect({ -300.f, 0.f }, { 1500.f, 900.f }));   // distanceToTarget panel ke liye 300px extra
    window.setView(gameView);

    // mazeTiles background
    sf::Texture backgroundTexture("maze.bmp");
    sf::Sprite backgroundSprite(backgroundTexture);

    // player
    sf::Texture playerTextureA("Player1_TextureA.png");
    sf::Texture playerTextureB("Player1_TextureB.png");
    Player player(playerTextureA);
    player.sprite.setScale(
        {
            40 / float(playerTextureA.getSize().x),
            40 / float(playerTextureA.getSize().y)
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

    sf::Text pausedTitle(font, "GAME PAUSED", 100);
    pausedTitle.setPosition({ 230, 330 });
    sf::Text pausedInstructions(font, "Press Esc or P to continue", 30);
    pausedInstructions.setPosition({ 290, 480 });



    // making the sidebar which tells game name and score
    sf::Texture panelBackgroundTexture("Game_banner.jpg");
    sf::Sprite rightPanelSprite(panelBackgroundTexture);
    rightPanelSprite.setPosition({ 900,0 });
    rightPanelSprite.setScale({ 300.f / panelBackgroundTexture.getSize().x, 900.f / panelBackgroundTexture.getSize().y });
    // abdullahameer added achievement board here
    sf::Sprite leftPanelSprite(panelBackgroundTexture);
    leftPanelSprite.setPosition({ -300.f, 0.f });
    leftPanelSprite.setScale({ 300.f / panelBackgroundTexture.getSize().x, 900.f / panelBackgroundTexture.getSize().y });
    sf::RectangleShape leftPanelOverlay({ 300.f, 900.f });          // text saaf parhne ke liye halka andhera
    leftPanelOverlay.setPosition({ -300.f, 0.f });
    leftPanelOverlay.setFillColor(sf::Color(0, 0, 0, 170));
    //
    sf::Text gameTitle(font, "PACMAN", 100);
    gameTitle.setFillColor(sf::Color::Yellow);
    gameTitle.setPosition({ 910, 200 });

    sf::Text scoreLabel(font, "Score: ", 50);
    scoreLabel.setFillColor(sf::Color::Red);
    scoreLabel.setPosition({ 950,300 });

    sf::Text scoreText(font, "0", 50);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({ 1080, 300 });

    sf::Text livesLabel(font, "Lives Left: ", 50);
    livesLabel.setFillColor(sf::Color::Red);
    livesLabel.setPosition({ 920, 350 });

    sf::Text livesText(font, "0", 50);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition({ 1130,350 });


    // Making the game over screen
    sf::RectangleShape gameOverOverlay({ 1200,900 });
    gameOverOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Text gameOverTitle(font, "GAME OVER", 200);
    sf::FloatRect gameOverTitleBounds = gameOverTitle.getLocalBounds();
    gameOverTitle.setOrigin({ gameOverTitleBounds.position.x + gameOverTitleBounds.size.x / 2.f, 0.f });   // horizontal centre
    gameOverTitle.setPosition({ 600.f, 200.f });

    sf::Text gameOverInstructions(font, "Press Space to start again", 50);
    sf::FloatRect gameOverInstructionsBounds = gameOverInstructions.getLocalBounds();
    gameOverInstructions.setOrigin({ gameOverInstructionsBounds.position.x + gameOverInstructionsBounds.size.x / 2.f, 0.f });
    gameOverInstructions.setPosition({ 600.f, 400.f });



    // variables (each declared ONCE)
    sf::Clock clock, timer_for_texture_change;
    float playerCollisionHalfSize = 15;         // playerCollisionHalfSize of Pac-Man's size (40 / 2) //another change here playerCollisionHalfSize=15 from now to make pac-man movement checking defined.
    char playerDirection = ' ';
    char requestedDirection = ' ';       // playerDirection the player asked for (turn happens at the next tile centre)
    bool gameHasStarted = false, showAlternatePlayerImage = false;
    bool isGamePaused = false;

    //seconds counter for leaderboard: 

    float runTimeSeconds = 0;           // seconds played in this run
    bool scoreWasSaved = false;     // so we save only once per game over
    loadScoreLeaderboard();                 // read the file when the game starts

    // frenzy mode (5.5) by hmza
    bool isFrenzyActive = false;      // true while frenzy is running
    float frenzyWaitSeconds = 0;       // seconds played since the last frenzy
    float frenzySecondsRemaining = 0;       // seconds of frenzy still distanceToTarget
    // places dots in every 1x1 area
    //pinkGhost.alive = blueGhost.alive = false;   // TEST: 2 ghosts pehle se mare hue   // testing for boss
    placeMazeDots();

    // ===== GHOST MOVEMENT SETUP =====
    // Done by MUHAMMAD KHIZAR REHMAN
    // edit by abd
    Ghost* ghosts[8] = { &redGhost, &pinkGhost, &blueGhost, &yellowGhost, &extraRedGhost, &extraPinkGhost, &extraBlueGhost, &extraYellowGhost };
    // edited by abd for makking ghost color same for some time 
    sf::Texture* ghostTextures[8] = { &redGhostTexture, &pinkGhostTexture, &blueGhostTexture, &yellowGhostTexture, &redGhostTexture, &pinkGhostTexture, &blueGhostTexture, &yellowGhostTexture };
    float ghostSizes[8] = { 30, 36, 36, 30, 30, 36, 36, 30 }; // har ghost ka size (jo upar setScale mein diya tha) aur next levels ke liye bhi ghosts
    bool areGhostsScared = false;
    // edit by abd
    int ghostRows[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile the ghost is on
    int ghostColumns[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // column of that tile
    int ghostTargetRows[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile it is walking to
    int ghostTargetColumns[8] = { 11, 13, 11, 13 ,11, 13, 11, 13 };   // column of that tile
    int ghostDirectionRows[8] = { 0, 0, 0, 0,0, 0, 0, 0 };       // last movementStep taken (row)
    int ghostDirectionColumns[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };       // last movementStep taken (column)
    int ghostCornerRows[8] = { 1, 1, 23, 23,1, 1, 23, 23 };     // corner each ghost runs to (row)
    int ghostCornerColumns[8] = { 23, 1, 23, 1, 23, 1, 23, 1 };     // corner each ghost runs to (column)
    int directionRowChanges[8] = { -1, 0, 1, 0,-1, 0, 1, 0 };      // the 4 ways: up, distanceToTarget, down, right
    int directionColumnChanges[8] = { 0, -1, 0, 1, 0, -1, 0, 1 };
    float ghostReleaseTimes[8] = { 0, 3, 6, 9, 12, 15, 18, 21 };    // Done by MUHAMMAD KHIZAR REHMAN: every ghost has its own release time
    float ghostReleaseTimer = 0, ghostModeTimer = 0;             // game time, mode timer
    bool isGhostScatterMode = false;                // false = chase, true = run to corner

    // ===== LIFE LOSS SETUP (ghost touches pac-man) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool ghostsNeedReset = false;                 // true = put ghosts back in the house
    int ghostHomeRows[8] = { 11, 10, 10, 11,11, 10, 10, 11 };   // home tile of each ghost (row)
    int ghostHomeColumns[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // home tile of each ghost (column)

    // ===== GATES SETUP (5.6) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    int entryGateRow = 23, entryGateColumn = 1;       // entry gate tile (lower distanceToTarget corner)
    int exitGateRow = 1, exitGateColumn = 23;       // exit gate tile (upper right corner)
    int pelletsCollected = 0;               // pellets eaten so far this level
    int startingPelletCount = 0;               // pellets that were on the stage at the start
    for (int i = 0; i < MAZE_COLUMN_COUNT * MAZE_ROW_COUNT; i++)
        if (mazeTiles[i] == 2)
            startingPelletCount++;
    float pelletRespawnTimers[MAZE_COLUMN_COUNT * MAZE_ROW_COUNT] = { 0 };   // respawn countdown of each eaten pellet
    float pelletRespawnSeconds = 300.f;               // an eaten pellet comes back after 300 sec (5 min)
    sf::RectangleShape gateShape({ 36.f, 36.f });
    gateShape.setOutlineThickness(3.f);
    gateShape.setOutlineColor(sf::Color::White);
    sf::Text gateInformationText(font, "", 25);
    gateInformationText.setFillColor(sf::Color::White);
    gateInformationText.setPosition({ 920.f, 600.f });

    // ===== GRAPE SETUP (5.10 extra life) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool grapeIsVisible = false;        // is the grape on the map right now?
    float grapeTimer = 0;          // seconds passed since the last grape spawn
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
    float bonusOrbTimer = 0;              // seconds passed since the bonus orb was eaten
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

    for (int i = 0; i < 3; i++)
        if (!deathSoundLoaded[i])
            std::cerr << "Could not load " << deathSoundFiles[i]
            << ". Check the file and its path.\n";

    // Each event advances its own playlist independently. Unavailable files
    // are skipped, while successfully loaded files retain the requested order.
    int nextLaneSound = 0;
    int nextDeathSound = 0;
    // Prevent the lane warning from restarting every frame while a ghost
    // remains in the same unobstructed row or column as Pac-Man.
    bool ghostLaneWasActive = false;

    // Do not start the pellet sound while another audio track/effect is playing.
    // This includes menu music, chase warnings, and death sounds.
    // ===== END AUDIO SETUP =====

    while (window.isOpen() && !nameEntered)
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            const sf::Event::TextEntered* te = event->getIf<sf::Event::TextEntered>();
            if (te != nullptr)
            {
                char c = (te->unicode < 128) ? (char)te->unicode : 0;
                int nameLength = 0;
                while (playerName[nameLength] != '\0')
                    nameLength++;

                if (te->unicode == 8)
                {
                    if (nameLength > 0)
                        playerName[nameLength - 1] = '\0';
                }
                else if (nameLength < PLAYER_INPUT_LENGTH &&
                    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
                {
                    playerName[nameLength] = c;
                    playerName[nameLength + 1] = '\0';
                }
            }

            const sf::Event::KeyPressed* ke = event->getIf<sf::Event::KeyPressed>();
            if (ke != nullptr)
            {
                if (ke->code == sf::Keyboard::Key::Enter && playerName[0] != '\0')
                    nameEntered = true;
            }
        }

        char displayedName[PLAYER_INPUT_LENGTH + 2];
        std::snprintf(displayedName, sizeof(displayedName), "%s_", playerName);
        nameText.setString(displayedName);
        window.clear();
        window.draw(nameTitle);
        window.draw(nameText);
        window.draw(nameHint);
        window.display();
    }

    // The menu/name-entry music stops as soon as the game starts.
    menuMusic.stop();

    if (playerName[0] == '\0')
        std::snprintf(playerName, sizeof(playerName), "%s", "PLAYER");
    loadPlayerAchievements(playerName);
    loadProgressLeaderboard();
    updateProgressLeaderboard(currentPlayerName, 0);
    clock.restart();
    // ===== END OF PLAYER NAME SCREEN =====

    while (window.isOpen())
    {
        // This makes sure you can pause game or exit window
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
                    isGamePaused = !isGamePaused;
            }
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
                if (gameHasStarted)
                {
                    if (isFrenzyActive)
                    {
                        frenzySecondsRemaining -= deltaTime;
                        if (frenzySecondsRemaining <= 0)      // 10 seconds over
                        {
                            isFrenzyActive = false;
                            frenzyWaitSeconds = 0;       // start counting the next minute
                        }
                    }
                    else
                    {
                        frenzyWaitSeconds += deltaTime;
                        if (frenzyWaitSeconds >= 60)     // 1 minute passed
                        {
                            isFrenzyActive = true;
                            frenzySecondsRemaining = 10;
                        }
                    }
                }
                int scoreMultiplier = isFrenzyActive ? 5 : 1;      // points multiplier
                // editimg by abd
                updatePowerUps(deltaTime);
                //abd
                updateAchievementPopup(deltaTime);
                if (levelBannerTimer > 0)
                    levelBannerTimer -= deltaTime;

                // ===== GRID-LOCKED MOVEMENT (pac-man moves only along a single row / column) =====
                // keys only set the WANTED playerDirection
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    requestedDirection = 'U';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    requestedDirection = 'L';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    requestedDirection = 'D';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    requestedDirection = 'R';

                sf::Vector2f playerPosition = player.sprite.getPosition();
                float movementStep = player.speed * deltaTime * getPlayerSpeedMultiplier();
                int row = int(playerPosition.y) / TILE_SIZE;
                int col = int(playerPosition.x) / TILE_SIZE;
                float tileCenterX = (col + 0.5f) * TILE_SIZE;   // centre of current tile
                float tileCenterY = (row + 0.5f) * TILE_SIZE;

                // turn only if that way is open and pac-man is at the tile centre (reversing is always allowed)
                if (requestedDirection != ' ' && requestedDirection != playerDirection)
                {
                    bool reverse = (playerDirection == 'U' && requestedDirection == 'D') || (playerDirection == 'D' && requestedDirection == 'U') ||
                        (playerDirection == 'L' && requestedDirection == 'R') || (playerDirection == 'R' && requestedDirection == 'L');
                    int targetRow = row + (requestedDirection == 'D') - (requestedDirection == 'U');
                    int targetColumn = col + (requestedDirection == 'R') - (requestedDirection == 'L');
                    bool atCentre = std::abs(playerPosition.x - tileCenterX) <= movementStep && std::abs(playerPosition.y - tileCenterY) <= movementStep;

                    if (!isWallTile(targetRow, targetColumn) && (reverse || atCentre))
                    {
                        if (!reverse)
                            playerPosition = { tileCenterX, tileCenterY };          // snap into the new lane
                        playerDirection = requestedDirection;
                        gameHasStarted = true;
                    }
                }

                // move along the current lane only
                float newX = playerPosition.x, newY = playerPosition.y;
                bool horizontal = (playerDirection == 'L' || playerDirection == 'R');

                if (playerDirection == 'U') { player.sprite.setRotation(sf::degrees(270)); newY -= movementStep; }
                else if (playerDirection == 'L') { player.sprite.setRotation(sf::degrees(180)); newX -= movementStep; }
                else if (playerDirection == 'D') { player.sprite.setRotation(sf::degrees(90));  newY += movementStep; }
                else if (playerDirection == 'R') { player.sprite.setRotation(sf::degrees(360)); newX += movementStep; }

                if (playerDirection != ' ')
                {
                    if (canPlayerMoveTo(newX, newY, playerCollisionHalfSize))
                        playerPosition = { newX, newY };
                    else if (horizontal)
                        playerPosition.x = tileCenterX;                    // hit a wall: stop at tile centre
                    else
                        playerPosition.y = tileCenterY;

                    if (horizontal) playerPosition.y = tileCenterY;        // lock to the row
                    else            playerPosition.x = tileCenterX;        // lock to the column
                }

                // starts the pacman animation if any movement key is pressed
                if (gameHasStarted)
                {
                    // write code for changing of texture
                    if (timer_for_texture_change.getElapsedTime().asSeconds() >= 0.3)
                    {
                        showAlternatePlayerImage = !showAlternatePlayerImage;
                        if (showAlternatePlayerImage)
                            player.sprite.setTexture(playerTextureA);
                        else
                            player.sprite.setTexture(playerTextureB);
                        timer_for_texture_change.restart();
                    }
                }

                player.sprite.setPosition(playerPosition);
                // ===== END GRID-LOCKED MOVEMENT =====


                for (int r = 0; r < MAZE_ROW_COUNT; r++)
                {
                    for (int c = 0; c < MAZE_COLUMN_COUNT; c++)
                    {
                        if (mazeTiles[r * MAZE_COLUMN_COUNT + c] == 2)
                        {
                            float dx = playerPosition.x - (c + 0.5f) * TILE_SIZE;
                            float dy = playerPosition.y - (r + 0.5f) * TILE_SIZE;
                            if (dx * dx + dy * dy < 15.f * 15.f)
                            {
                                mazeTiles[r * MAZE_COLUMN_COUNT + c] = 0;
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
                                pelletRespawnTimers[r * MAZE_COLUMN_COUNT + c] = pelletRespawnSeconds;       // Done by MUHAMMAD KHIZAR REHMAN: it will come back later
                            }
                        }
                    }
                }
                // orbits  and levels speed added by abdullahameer
                player.score += collectPowerUps(playerPosition) * scoreMultiplier;
                checkScoreAchievements(player.score);

                // ===== PELLET RESPAWN + EXIT GATE (5.6) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                for (int k = 0; k < MAZE_COLUMN_COUNT * MAZE_ROW_COUNT; k++)
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
                if (pelletsCollected >= startingPelletCount && qx * qx + qy * qy < 18.f * 18.f)
                {
                    level++;
                    // edited by abd
                    checkFastLevelAchievement(runTimeSeconds - levelStartTime);
                    levelStartTime = runTimeSeconds;
                    updateProgressLeaderboard(currentPlayerName, level - 1);   // levels cleared so far
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
                    ghostsNeedReset = true;                                 // ghosts wapas house mein

                    pelletsCollected = 0;                                    // Done by MUHAMMAD KHIZAR REHMAN: new level, new pellet count
                    for (int k = 0; k < MAZE_COLUMN_COUNT * MAZE_ROW_COUNT; k++)
                        pelletRespawnTimers[k] = 0;

                    bonusOrbIsVisible = true;                                  // Done by MUHAMMAD KHIZAR REHMAN: bonus orb is back on a new level
                    bonusOrbTimer = 0;

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
                    areGhostsScared = false;
                }
                // ===== END LEVEL UP =====

                // kill-ghost orb chal raha ho to sab ghosts ek jese, khatam ho to wapas asli
                bool scaredNow = isGhostKillPowerActive();
                if (scaredNow != areGhostsScared)
                {
                    areGhostsScared = scaredNow;
                    for (int i = 0; i < 8; i++)
                    {
                        if (ghosts[i]->isBoss)
                            continue;                 // boss keeps its red boss look
                        sf::Texture& t = areGhostsScared ? blueGhostTexture : *ghostTextures[i];
                        ghosts[i]->sprite.setTexture(t, true);
                        ghosts[i]->sprite.setScale({ ghostSizes[i] / float(t.getSize().x),
                                                 ghostSizes[i] / float(t.getSize().y) });
                        ghosts[i]->sprite.setOrigin({ t.getSize().x / 2.f, t.getSize().y / 2.f });
                    }
                }



                // ===== GHOST MOVEMENT =====
                // Done by MUHAMMAD KHIZAR REHMAN

                // after a hit, put the ghosts back in the house and restart their timers
                if (ghostsNeedReset)
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
                    ghostReleaseTimer = 0;
                    ghostModeTimer = 0;
                    isGhostScatterMode = false;
                    gameHasStarted = false;
                    ghostsNeedReset = false;
                }

                if (gameHasStarted)
                {
                    ghostReleaseTimer += deltaTime * (isFrenzyActive ? 2.f : 1.f);   // frenzy: ghosts leave the house twice as fast
                    ghostModeTimer += deltaTime;
                }
                if (isGhostScatterMode && ghostModeTimer >= 7)
                {
                    isGhostScatterMode = false; ghostModeTimer = 0;
                }        // 7 sec corner run
                else if (!isGhostScatterMode && ghostModeTimer >= 20)
                {
                    isGhostScatterMode = true; ghostModeTimer = 0;
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
                    if (!gameHasStarted || ghostReleaseTimer < ghostReleaseTimes[i] || !ghosts[i]->alive)
                        continue;

                    sf::Vector2f ghostPosition = ghosts[i]->sprite.getPosition();
                    float targetPositionX = (ghostTargetColumns[i] + 0.5f) * TILE_SIZE;
                    float targetPositionY = (ghostTargetRows[i] + 0.5f) * TILE_SIZE;
                    float movementStep = ghosts[i]->speed * deltaTime * levelSpeedMultiplier;
                    float distanceToTarget = std::abs(targetPositionX - ghostPosition.x) + std::abs(targetPositionY - ghostPosition.y);

                    // still walking to the next tile
                    if (distanceToTarget > movementStep)
                    {
                        ghostPosition.x += ghostDirectionColumns[i] * movementStep;
                        ghostPosition.y += ghostDirectionRows[i] * movementStep;
                        ghosts[i]->sprite.setPosition(ghostPosition);
                        continue;
                    }

                    // reached the tile centre, so snap there and decide the next turn
                    ghosts[i]->sprite.setPosition({ targetPositionX, targetPositionY });
                    ghostRows[i] = ghostTargetRows[i];
                    ghostColumns[i] = ghostTargetColumns[i];
                    // making movements like other ghosts
                    int ghostType = i % 4;      // naye ghosts purane 4 ki tarah chalein
                    // 1) choose the target square
                    int targetRow, targetColumn;
                    bool ghostIsInHome = (ghostRows[i] >= 9 && ghostRows[i] <= 11 && ghostColumns[i] >= 11 && ghostColumns[i] <= 13);
                    if (ghostIsInHome)
                    {
                        targetRow = 8; targetColumn = 12;                   // head for the house door
                    }
                    else if (ghosts[i]->isBoss)
                    {
                        targetRow = playerRow; targetColumn = playerColumn;                  // boss always chases pac-man
                    }
                    else if (isGhostScatterMode)
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
                        int mirroredTargetRow = playerRow + 2 * playerDirectionRow, mirroredTargetColumn = playerColumn + 2 * playerDirectionColumn;  // 2 squares ahead
                        targetRow = 2 * mirroredTargetRow - ghostRows[0];                     // red -> there, doubled
                        targetColumn = 2 * mirroredTargetColumn - ghostColumns[0];
                    }
                    else
                    {
                        int rowDistanceToPlayer = playerRow - ghostRows[i], columnDistanceToPlayer = playerColumn - ghostColumns[i];
                        if (rowDistanceToPlayer * rowDistanceToPlayer + columnDistanceToPlayer * columnDistanceToPlayer < 64)        // closer than 8 squares
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
                    int bestDirectionIndex = -1, bestTargetDistance = 999999;
                    int validDirectionIndices[4], validDirectionCount = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        if (directionRowChanges[k] == -ghostDirectionRows[i] && directionColumnChanges[k] == -ghostDirectionColumns[i])
                            continue;                                   // no turning back
                        int candidateRow = ghostRows[i] + directionRowChanges[k], candidateColumn = ghostColumns[i] + directionColumnChanges[k];
                        if (candidateRow < 0 || candidateRow >= MAZE_ROW_COUNT || candidateColumn < 0 || candidateColumn >= MAZE_COLUMN_COUNT)
                            continue;
                        if (mazeTiles[candidateRow * MAZE_COLUMN_COUNT + candidateColumn] == 1 || mazeTiles[candidateRow * MAZE_COLUMN_COUNT + candidateColumn] == -1)
                            continue;                                   // wall (or tunnel edge)
                        if (!ghostIsInHome && candidateRow >= 9 && candidateRow <= 11 && candidateColumn >= 11 && candidateColumn <= 13)
                            continue;                                   // can't go back in the house
                        validDirectionIndices[validDirectionCount++] = k;
                        int d = (candidateRow - targetRow) * (candidateRow - targetRow) + (candidateColumn - targetColumn) * (candidateColumn - targetColumn);
                        if (d < bestTargetDistance || (d == bestTargetDistance && std::rand() % 2 == 0))
                        {
                            bestTargetDistance = d;
                            bestDirectionIndex = k;
                        }
                    }

                    // random ghost decision: 8% of the time a normal ghost (not the boss,
                    // not leaving the house) takes a random open way at a junction
                    if (validDirectionCount > 1 && !ghostIsInHome && !ghosts[i]->isBoss && std::rand() % 100 < 8)
                        bestDirectionIndex = validDirectionIndices[std::rand() % validDirectionCount];

                    if (bestDirectionIndex == -1)              // dead end: the only time it may turn back
                    {
                        ghostDirectionRows[i] = -ghostDirectionRows[i];
                        ghostDirectionColumns[i] = -ghostDirectionColumns[i];
                    }
                    else
                    {
                        ghostDirectionRows[i] = directionRowChanges[bestDirectionIndex];
                        ghostDirectionColumns[i] = directionColumnChanges[bestDirectionIndex];
                    }
                    ghostTargetRows[i] = ghostRows[i] + ghostDirectionRows[i];
                    ghostTargetColumns[i] = ghostColumns[i] + ghostDirectionColumns[i];
                }
                // boss mode add by abd inwhich when two ghosts are kill so other 2 ghosts turn into boss mode
                // ===== BOSS MODE (5.8) =====
                int livingGhostCount = 0;
                for (int i = 0; i < activeGhostCount; i++)
                    if (ghosts[i]->alive)
                        livingGhostCount++;

                if (livingGhostCount <= 1)                          // only 1 ghost distanceToTarget: it becomes the boss
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
                        int livingGhostCountNow = 0;
                        for (int k = 0; k < activeGhostCount; k++)
                            if (ghosts[k]->alive)
                                livingGhostCountNow++;

                        if (isGhostKillPowerActive() && !ghosts[i]->isBoss && livingGhostCountNow > 1)
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
                            playNextAvailableSound(deathSounds, deathSoundLoaded, 3, nextDeathSound);

                            // koi orb active nahi: pehle wala lives wala code (bilkul wese hi)
                            player.lives--;                         // lose one life
                            player.sprite.setPosition({ 54, 846 }); // pac-man goes back to start
                            playerDirection = ' ';
                            requestedDirection = ' ';
                            resetPowerUps(false);                        // orb ke timers band (sirf yeh ek line nayi hai)
                            ghostsNeedReset = true;                              // ghosts go back to the house
                            break;                                   // only one life per hit
                        }
                        // agar invincible chal raha hai to kuch nahi hota
                    }
                }

                // ===== GHOST ENTERS PAC-MAN'S LANE AUDIO WARNING =====
                // Check active, living ghosts after their movement for this frame.
                bool ghostInPlayerLane = false;
                if (gameHasStarted && player.lives > 0)
                {
                    for (int i = 0; i < activeGhostCount; i++)
                    {
                        if (!ghosts[i]->alive || ghostReleaseTimer < ghostReleaseTimes[i])
                            continue;

                        if (isGhostInClearLane(playerPosition, ghosts[i]->sprite.getPosition()))
                        {
                            ghostInPlayerLane = true;
                            break;
                        }
                    }
                }

                // Play once when a ghost enters the lane. It will be allowed
                // to play again only after all ghosts leave Pac-Man's lane.
                if (ghostInPlayerLane && !ghostLaneWasActive)
                {
                    // Chase warning has priority: interrupt pellet-eating audio.
                    pelletSound.stop();

                    // Play the next sound in the chase playlist whenever a
                    // ghost newly enters Pac-Man's unobstructed row or column.
                    playNextAvailableSound(laneWarningSounds, laneSoundLoaded, 4, nextLaneSound);
                }
                ghostLaneWasActive = ghostInPlayerLane;
                // ===== END GHOST LANE AUDIO WARNING =====

                // ===== GRAPE: extra life every 5 minutes (5.10) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                if (gameHasStarted)
                    grapeTimer += deltaTime;
                if (grapeTimer >= grapeSpawnInterval)         // 5 minutes passed
                {
                    grapeIsVisible = true;          // grape appears
                    grapeTimer = 0;
                }
                if (grapeIsVisible)
                {
                    float grapeDistanceX = playerPosition.x - 450.f, grapeDistanceY = playerPosition.y - 306.f;
                    if (grapeDistanceX * grapeDistanceX + grapeDistanceY * grapeDistanceY < 20.f * 20.f)     // pac-man touches the grape
                    {
                        player.lives++;                     // extra life
                        grapeIsVisible = false;                          // grape disappears
                    }
                }

                // ===== SPECIAL BONUS ORB (5.2) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                if (!bonusOrbIsVisible)
                {
                    bonusOrbTimer += deltaTime;               // count the seconds since it was eaten
                    if (bonusOrbTimer >= bonusOrbRespawnSeconds)          // 2 minutes passed
                    {
                        bonusOrbIsVisible = true;          // the orb is back
                        bonusOrbTimer = 0;
                    }
                }
                else
                {
                    float bonusOrbDistanceX = playerPosition.x - (bonusOrbColumn + 0.5f) * TILE_SIZE;
                    float bonusOrbDistanceY = playerPosition.y - (bonusOrbRow + 0.5f) * TILE_SIZE;
                    if (bonusOrbDistanceX * bonusOrbDistanceX + bonusOrbDistanceY * bonusOrbDistanceY < 18.f * 18.f)     // pac-man touches the orb
                    {
                        player.score += 1000 * scoreMultiplier;        // 1000 bonus points
                        bonusOrbIsVisible = false;                          // the orb disappears
                        bonusOrbTimer = 0;                              // the 2 minute countdown starts
                    }
                }
                // ===== END OF GHOST MOVEMENT =====

                // 3. DRAW
            }

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


            // take the current score and store it in score value
            char scoreDigitsText[12], scoreDigits[12];
            int scoreTextLength = 0;
            int scoreNumber = player.score, d = 0;
            if (scoreNumber == 0)
            {
                scoreDigits[d] = '0';
                d++;
            }
            while (scoreNumber > 0)
            {
                scoreDigits[d] = '0' + scoreNumber % 10;
                d++;
                scoreNumber /= 10;
            }
            while (d > 0)
            {
                d--;
                scoreDigitsText[scoreTextLength] = scoreDigits[d];
                scoreTextLength++;
            }
            scoreDigitsText[scoreTextLength] = '\0';
            scoreText.setString(scoreDigitsText);

            //take current lives and put then in lives value
            char livesDigitsText[12], livesDigits[12];
            int livesTextLength = 0;
            int livesNumber = player.lives, d2 = 0;
            if (livesNumber == 0)
            {
                livesDigits[d2] = '0';
                d2++;
            }
            while (livesNumber > 0)
            {
                livesDigits[d2] = '0' + livesNumber % 10;
                d2++;
                livesNumber /= 10;
            }
            while (d2 > 0)
            {
                d2--;
                livesDigitsText[livesTextLength] = livesDigits[d2];
                livesTextLength++;
            }
            livesDigitsText[livesTextLength] = '\0';
            livesText.setString(livesDigitsText);



            window.clear();
            backgroundSprite.setColor(isFrenzyActive ? sf::Color(150, 90, 200) : sf::Color::White);   // tint background in frenzy
            window.draw(backgroundSprite);
            window.draw(rightPanelSprite);
            window.draw(gameTitle);
            window.draw(scoreLabel);
            window.draw(scoreText);
            window.draw(livesLabel);
            window.draw(livesText);
            drawPowerUpTimers(window, font);

            // TEST: red squares over wall cells (delete when walls are finished)
            // This is rayan, Dont delete this, i am using it to build mazeTiles

            sf::RenderStates glowStates;
            glowStates.blendMode = sf::BlendAdd;   // colors add up, which gives the glow

            // outer glow, then inner glow (additive)
            if (isFrenzyActive)   // frenzy colors: pink / purple
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
            drawMazeWallLayer(window, 10.f, isFrenzyActive ? sf::Color(255, 200, 255) : sf::Color(255, 248, 190), sf::RenderStates::Default);
            //draw dots
            sf::CircleShape dot(3.f);
            dot.setFillColor(sf::Color::Yellow);

            for (int r = 0; r < MAZE_ROW_COUNT; r++)
            {
                for (int c = 0; c < MAZE_COLUMN_COUNT; c++)
                {
                    if (mazeTiles[r * MAZE_COLUMN_COUNT + c] == 2)
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
            if (pelletsCollected >= startingPelletCount)
                gateShape.setFillColor(sf::Color(50, 255, 80, 160));      // exit open: green
            else
                gateShape.setFillColor(sf::Color(255, 50, 50, 140));      // exit locked: red
            window.draw(gateShape);

            char sidebarText[64];
            std::snprintf(sidebarText, sizeof(sidebarText),
                "Level: %d\nPellets: %d/%d\nEXIT: %s",
                level, pelletsCollected, startingPelletCount, pelletsCollected >= startingPelletCount ? "OPEN" : "LOCKED");
            gateInformationText.setString(sidebarText);
            window.draw(gateInformationText);

            if (grapeIsVisible)
                window.draw(grapeSprite);

            if (bonusOrbIsVisible)
                window.draw(bonusOrbShape);   // Done by MUHAMMAD KHIZAR REHMAN: special bonus orb (5.2)

            // abdullahameer code here

            drawPowerUps(window, runTimeSeconds);
            window.draw(leftPanelSprite);
            drawAchievementBoard(window, font);
            drawProgressLeaderboard(window, font);

            if (isFrenzyActive)
            {
                char frenzyTextValue[32];
                std::snprintf(frenzyTextValue, sizeof(frenzyTextValue), "FRENZY x5: %d",
                    int(frenzySecondsRemaining) + 1);
                sf::Text frenzyText(font, frenzyTextValue, 30);
                frenzyText.setFillColor(sf::Color::Magenta);
                frenzyText.setPosition({ 930.f, 560.f });
                window.draw(frenzyText);
            }
        }

        window.draw(player.sprite);   // last, so he is on top
        if (isGamePaused)
        {
            window.draw(overlay);
            window.draw(pausedTitle);
            window.draw(pausedInstructions);
        }


        for (int i = 0; i < activeGhostCount; i++)
            if (ghosts[i]->alive) window.draw(ghosts[i]->sprite);

        if (levelBannerTimer > 0)                                    // LEVEL n screen
        {
            char levelText[24];
            std::snprintf(levelText, sizeof(levelText), "LEVEL %d", level);
            sf::Text lvlText(font, levelText, 120);
            lvlText.setFillColor(sf::Color::Yellow);
            lvlText.setPosition({ 250.f, 380.f });
            window.draw(lvlText);
        }


        if (player.lives <= 0)
        {
            // save the score if not already done (leaderboard code by hmza):
            if (!scoreWasSaved)
            {
                addScoreToLeaderboard(playerName, player.score, int(runTimeSeconds));
                scoreWasSaved = true;
            }

            window.draw(gameOverOverlay);
            window.draw(gameOverTitle);
            window.draw(gameOverInstructions);
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
                isFrenzyActive = false;      // frenzy resets on restart (hmza)
                frenzyWaitSeconds = 0;
                // allow the next game over to save again
                 // abdullahameer code
                resetPowerUps(true);

                // Done by MUHAMMAD KHIZAR REHMAN: gates and grape start fresh
                pelletsCollected = 0;
                for (int k = 0; k < MAZE_COLUMN_COUNT * MAZE_ROW_COUNT; k++)
                    pelletRespawnTimers[k] = 0;
                grapeIsVisible = false;
                grapeTimer = 0;
                bonusOrbIsVisible = true;             // Done by MUHAMMAD KHIZAR REHMAN: bonus orb starts fresh
                bonusOrbTimer = 0;

                // ===== LEVEL aur GHOSTS reset (abdullahameer) =====
                level = 1;
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
                areGhostsScared = false;
                ghostsNeedReset = true;                    // ghosts wapas house mein
                // ===== END RESET =====
            }


        }
        window.display();

    }
}

void placeMazeDots()
{
    for (int r = 0; r < MAZE_ROW_COUNT; r++)
    {
        for (int c = 0; c < MAZE_COLUMN_COUNT; c++)
        {
            if (mazeTiles[r * MAZE_COLUMN_COUNT + c] != 0)
                continue;

            // no dots inside the ghost house
            bool inHouse = (r >= 9 && r <= 12 && c >= 10 && c <= 14);
            if (!inHouse)
                mazeTiles[r * MAZE_COLUMN_COUNT + c] = 2;
        }
    }
}

void drawMazeWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states)
{
    sf::RectangleShape piece;
    piece.setFillColor(color);

    float margin = (TILE_SIZE - thickness) / 2.f;   // gap between tile edge and the shape

    for (int row = 0; row < MAZE_ROW_COUNT; row++)
    {
        for (int col = 0; col < MAZE_COLUMN_COUNT; col++)
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
