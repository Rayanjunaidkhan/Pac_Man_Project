#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <fstream>
#include <string>
#include <optional> // Abd
#include <iostream>
#include <cmath>   // Done by MUHAMMAD KHIZAR REHMAN
#include <cstdlib> // rand / srand (random ghost decisions, random orb spawning)
#include <ctime>   // time stamps on leaderboard runs, seed for rand

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
void place_dots();
// Draws one glowing layer over the maze walls.
void drawWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states);

// making constants coz these are fixed mapping values of game.
const int CELL = 36;   // size of one square, in pixels
const int COLS = 25;   // how many squares across
const int ROWS = 25;   // how many squares down


// 0 for path with point, 1 for wall, -1 for path without point
int maze[COLS * ROWS] = {
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
const int   ORB_COUNT = 4;
const int   ORB_TILE = 36;
const int  orbHomeRow[ORB_COUNT] = { 1, 1, 13, 23 };   // where each orb starts every level
const int  orbHomeCol[ORB_COUNT] = { 1, 23, 12, 23 };
int  orbRow[ORB_COUNT] = { 1, 1, 13, 23 };
int  orbCol[ORB_COUNT] = { 1, 23, 12, 23 };
int  orbType[ORB_COUNT] = { 1, 2, 3, 1 };    // 1 = speed, 2 = invincible, 3 = kill-ghost
bool orbOn[ORB_COUNT] = { true, true, true, true };
const float ORB_RESPAWN_TIME = 60.f;
float orbRespawnTimer[ORB_COUNT] = { 0.f, 0.f, 0.f, 0.f };

const float SPEED_TIME = 10.f;
const float INVINCIBLE_TIME = 10.f;
const float KILL_TIME = 10.f;
const float SPEED_BOOST = 1.6f;

float speedTimer = 0, invTimer = 0, killTimer = 0;
// working for next levels by abd
int   level = 1;               // abhi kaun sa level hai
float levelSpeed = 1.f;        // har level par x1.10
int   ghostCount = 4;          // abhi kitne ghosts khel mein hain
const int MAX_GHOSTS = 8;
float levelBanner = 0;         // "LEVEL 2" screen ka timer
// Chooses the display color for a power-up based on its type.
sf::Color orbColor(int type)
{
    if (type == 1)
        return sf::Color(0, 220, 255);
    if (type == 2)
        return sf::Color(255, 215, 0);
    return sf::Color(255, 40, 40);
}

// Moves a power-up to a random open tile that is not already in use.
void orbRandomTile(int o)
{
    for (int tries = 0; tries < 200; tries++)
    {
        int r = std::rand() % ROWS;
        int c = std::rand() % COLS;

        if (maze[r * COLS + c] != 0 && maze[r * COLS + c] != 2)
            continue;                                   // wall or tunnel edge
        if (r >= 9 && r <= 12 && c >= 10 && c <= 14)
            continue;                                   // ghost house
        if (r == 8 && (c == 11 || c == 12))
            continue;                                   // bonus orb + grape tiles
        if ((r == 23 && c == 1) || (r == 1 && c == 23))
            continue;                                   // entry / exit gate

        bool taken = false;
        for (int k = 0; k < ORB_COUNT; k++)
            if (k != o && orbOn[k] && orbRow[k] == r && orbCol[k] == c)
                taken = true;
        if (taken)
            continue;

        orbRow[o] = r;
        orbCol[o] = c;
        return;
    }
}

// Counts down active power-ups and brings collected orbs back later.
void orbsUpdate(float dt)
{
    if (speedTimer > 0)
        speedTimer = std::max(0.f, speedTimer - dt);
    if (invTimer > 0)
        invTimer = std::max(0.f, invTimer - dt);
    if (killTimer > 0)
        killTimer = std::max(0.f, killTimer - dt);

    // Each collected power-up orb respawns 60 seconds after it was collected,
    // on a random tile.
    for (int o = 0; o < ORB_COUNT; o++)
    {
        if (!orbOn[o] && orbRespawnTimer[o] > 0.f)
        {
            orbRespawnTimer[o] -= dt;
            if (orbRespawnTimer[o] <= 0.f)
            {
                orbRespawnTimer[o] = 0.f;
                orbRandomTile(o);
                orbOn[o] = true;
            }
        }
    }
}
// Returns the player's speed multiplier, including the current level.
float orbSpeedMultiplier()
{
    return (speedTimer > 0 ? SPEED_BOOST : 1.f) * levelSpeed;
}

// Tells whether the shield power-up is still active.
bool  orbInvincible()
{
    return invTimer > 0;
}
// Tells whether the power-up that can defeat ghosts is active.
bool  orbKillActive()
{
    return killTimer > 0;
}


// Collects nearby power-ups, starts their timers, and returns bonus points.
int orbsPickup(sf::Vector2f pos)
{
    int bonus = 0;
    for (int o = 0; o < ORB_COUNT; o++)
    {
        if (!orbOn[o]) continue;
        float dx = pos.x - (orbCol[o] + 0.5f) * ORB_TILE;
        float dy = pos.y - (orbRow[o] + 0.5f) * ORB_TILE;
        if (dx * dx + dy * dy < 18.f * 18.f)
        {
            orbOn[o] = false;
            orbRespawnTimer[o] = ORB_RESPAWN_TIME;
            if (orbType[o] == 1) speedTimer = SPEED_TIME;
            else if (orbType[o] == 2) invTimer = INVINCIBLE_TIME;
            else killTimer = KILL_TIME;
            bonus += 50;
        }
    }
    return bonus;
}

// Clears power-up timers and optionally returns every orb to its start tile.
void orbsReset(bool respawnOrbs)
{
    speedTimer = 0;
    invTimer = 0;
    killTimer = 0;
    if (respawnOrbs)
    {
        for (int o = 0; o < ORB_COUNT; o++)
        {
            orbOn[o] = true;
            orbRespawnTimer[o] = 0.f;
            orbRow[o] = orbHomeRow[o];      // back to the starting tile
            orbCol[o] = orbHomeCol[o];
        }
    }
}


// Draws each power-up orb that is currently visible.
void orbsDraw(sf::RenderWindow& window, float t)
{
    sf::CircleShape shape;
    for (int o = 0; o < ORB_COUNT; o++)
    {
        if (!orbOn[o]) continue;
        float rad = 9.f + 2.f * std::sin(t * 6.f);
        shape.setRadius(rad);
        shape.setOrigin({ rad, rad });
        shape.setPosition({ (orbCol[o] + 0.5f) * ORB_TILE, (orbRow[o] + 0.5f) * ORB_TILE });
        shape.setFillColor(orbColor(orbType[o]));
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
    }
}

// Shows the remaining time for each active power-up on the screen.
void orbsDrawHud(sf::RenderWindow& window, const sf::Font& font)
{
    std::string s = "";
    if (speedTimer > 0)
        s += "SPEED: " + std::to_string(int(speedTimer) + 1) + "\n";
    if (invTimer > 0)
        s += "SHIELD: " + std::to_string(int(invTimer) + 1) + "\n";
    if (killTimer > 0)
        s += "KILL: " + std::to_string(int(killTimer) + 1) + "\n";
    if (s.empty()) return;
    sf::Text text(font, s, 30);
    text.setFillColor(sf::Color::Red);
    text.setPosition({ 930.f, 430.f });
    window.draw(text);
}
// ===== END OF ORB CODE =====
// 
// 
// 
// 
// abdillahameer added achievement board
// ===== ACHIEVEMENTS + TOP 3 PLAYERS (Abdullah Ameer) =====
const int   ACH_COUNT = 5;
const float RECORD_TIME = 120.f;
const float ACH_X = -280.f;
const float ACH_Y = 330.f;
const int   LEVEL_TOTAL = 5;
const float LB_X = -280.f;
const float LB_Y = 60.f;
const int   LB_MAX = 50;

const char* achName[ACH_COUNT] = { "SCORE 500", "SCORE 1500", "SCORE 3000", "GHOST HUNTER", "SPEED RUNNER" };
bool  achDone[ACH_COUNT] = { false, false, false, false, false };
float achPopupTimer = 0;
std::string achPopupText = "";
std::string achPlayer = "PLAYER";
float levelStartTime = 0;
// Abdullah Ameer added the achievements board.
// Declares the leaderboard update function used when achievements change.
void lbUpdate(const std::string& name, int count);   // forward declaration

// Counts how many achievements the current player has unlocked.
int achCount()
{
    int c = 0;
    for (int i = 0; i < ACH_COUNT; i++)
        if (achDone[i]) c++;
    return c;
}
// Saves the current player's achievement progress to a file.
void achSave()
{
    std::ofstream f("ach_" + achPlayer + ".txt");
    for (int i = 0; i < ACH_COUNT; i++)
        f << (achDone[i] ? 1 : 0) << " ";
}

// Loads a player's saved achievements and clears missing ones.
void achLoad(const std::string& name)
{
    achPlayer = name;
    for (int i = 0; i < ACH_COUNT; i++)
        achDone[i] = false;
    std::ifstream f("ach_" + achPlayer + ".txt");
    for (int i = 0; i < ACH_COUNT; i++)
    {
        int v = 0;
        if (f >> v) achDone[i] = (v == 1);
    }
}

// Unlocks an achievement once, shows a message, and saves the result.
void achUnlock(int i)
{
    if (achDone[i]) return;
    achDone[i] = true;
    achPopupText = std::string("UNLOCKED: ") + achName[i];
    achPopupTimer = 3.f;
    achSave();
    lbUpdate(achPlayer, 0);
}

// Unlocks score achievements when the player reaches each target.
void achCheckScore(int score)
{
    if (score >= 500)  achUnlock(0);
    if (score >= 1500) achUnlock(1);
    if (score >= 3000) achUnlock(2);
}

// Unlocks the achievement for eating a ghost.
void achGhostEaten() { achUnlock(3); }

// Unlocks the speed-run achievement when a level is cleared in time.
void achLevelCleared(float seconds)
{
    if (seconds <= RECORD_TIME) achUnlock(4);
}

// Decreases the time left for the achievement message.
void achUpdate(float dt)
{
    if (achPopupTimer > 0) achPopupTimer -= dt;
}

// Draws the older achievement list and any unlock message.
void achDrawOld(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text who(font, "PLAYER: " + achPlayer, 22);
    who.setFillColor(sf::Color::White);
    who.setPosition({ ACH_X, ACH_Y });
    window.draw(who);

    sf::Text title(font, "ACHIEVEMENTS", 24);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({ ACH_X, ACH_Y + 26.f });
    window.draw(title);

    for (int i = 0; i < ACH_COUNT; i++)
    {
        std::string line = std::string(achDone[i] ? "[X] " : "[ ] ") + achName[i];
        sf::Text t(font, line, 20);
        t.setFillColor(achDone[i] ? sf::Color(255, 215, 0) : sf::Color(160, 160, 160));
        t.setPosition({ ACH_X, ACH_Y + 58.f + i * 22.f });
        window.draw(t);
    }

    if (achPopupTimer > 0)
    {
        sf::Text pop(font, achPopupText, 36);
        pop.setFillColor(sf::Color::Yellow);
        pop.setOutlineThickness(3.f);
        pop.setOutlineColor(sf::Color::Black);
        pop.setPosition({ 200.f, 20.f });
        window.draw(pop);
    }
}
std::string lbName[LB_MAX];
int lbLevels[LB_MAX];   // levels cleared (0..LEVEL_TOTAL)
int lbAch[LB_MAX];      // achievements unlocked (0..ACH_COUNT)
int lbCount = 0;

// Adds a player's cleared levels and achievements for ranking.
int lbTotal(int i) { return lbLevels[i] + lbAch[i]; }

// Counts the achievements saved in the named player's file.
int achCountOf(const std::string& name)
{
    std::ifstream a("ach_" + name + ".txt");
    int c = 0, v = 0;
    for (int i = 0; i < ACH_COUNT && (a >> v); i++)
        if (v == 1) c++;
    return c;
}

// Saves the progress leaderboard to a file.
void lbSave()
{
    std::ofstream f("top_progress.txt");
    for (int i = 0; i < lbCount; i++)
        f << lbName[i] << " " << lbLevels[i] << "\n";
}

// Sorts the leaderboard from highest progress to lowest.
void lbSort()
{
    for (int i = 0; i < lbCount - 1; i++)
    {
        for (int j = 0; j < lbCount - 1 - i; j++)
        {
            bool swapNow = lbTotal(j) < lbTotal(j + 1) ||
                (lbTotal(j) == lbTotal(j + 1) && lbAch[j] < lbAch[j + 1]);
            if (swapNow)
            {
                std::string s = lbName[j];
                lbName[j] = lbName[j + 1];
                lbName[j + 1] = s;

                int t = lbLevels[j];
                lbLevels[j] = lbLevels[j + 1];
                lbLevels[j + 1] = t;

                t = lbAch[j];
                lbAch[j] = lbAch[j + 1];
                lbAch[j + 1] = t;
            }
        }
    }
}

// Loads saved leaderboard entries and sorts them.
void lbLoad()
{
    lbCount = 0;
    std::ifstream f("top_progress.txt");
    while (lbCount < LB_MAX && f >> lbName[lbCount] >> lbLevels[lbCount])
        lbCount++;

    for (int i = 0; i < lbCount; i++)
    {
        if (lbLevels[i] > LEVEL_TOTAL) lbLevels[i] = LEVEL_TOTAL;
        lbAch[i] = achCountOf(lbName[i]);
    }
    lbSort();
}

// Adds or updates a player's level progress and saves the ranking.
void lbUpdate(const std::string& name, int levels)
{
    if (levels > LEVEL_TOTAL)
        levels = LEVEL_TOTAL;

    int idx = -1;
    for (int i = 0; i < lbCount; i++)
        if (lbName[i] == name) { idx = i; break; }

    if (idx == -1)
    {
        if (lbCount >= LB_MAX) return;
        idx = lbCount++;
        lbName[idx] = name;
        lbLevels[idx] = 0;
    }
    if (levels > lbLevels[idx])
        lbLevels[idx] = levels;
    lbAch[idx] = achCountOf(name);

    lbSort();
    lbSave();
}

// Draws the older compact progress leaderboard.
void lbDrawOld(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text title(font, "TOP 3 PLAYERS", 22);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({ LB_X, LB_Y });
    window.draw(title);

    for (int i = 0; i < 3 && i < lbCount; i++)
    {
        std::string line = std::to_string(i + 1) + ". " + lbName[i] + "  " +
            std::to_string(lbLevels[i]) + "/" + std::to_string(LEVEL_TOTAL);
        sf::Text t(font, line, 20);
        t.setFillColor(i == 0 ? sf::Color(255, 215, 0) : sf::Color::White);
        t.setPosition({ LB_X, LB_Y + 26.f + i * 24.f });
        window.draw(t);
    }
}




// ===== PRETTY BOARDS (presentable look) =====
// Draws left-aligned text at the given screen position.
void panelText(sf::RenderWindow& w, const sf::Font& font, const std::string& s, unsigned int size, sf::Color col, float x, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    t.setPosition({ x, y });
    w.draw(t);
}

// Draws text centered around the given horizontal position.
void panelTextCentered(sf::RenderWindow& w, const sf::Font& font, const std::string& s, unsigned int size, sf::Color col, float cx, float y)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, 0.f });
    t.setPosition({ cx, y });
    w.draw(t);
}

// Draws a dark panel with a gold outline.
void panelBox(sf::RenderWindow& w, float x, float y, float width, float height)
{
    sf::RectangleShape r({ width, height });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(0, 0, 0, 190));
    r.setOutlineThickness(2.f);
    r.setOutlineColor(sf::Color(255, 215, 0));
    w.draw(r);
}

// Draws a thin gold divider inside a panel.
void panelLine(sf::RenderWindow& w, float x, float y, float width)
{
    sf::RectangleShape r({ width, 2.f });
    r.setPosition({ x, y });
    r.setFillColor(sf::Color(255, 215, 0, 160));
    w.draw(r);
}


// Draws the three highest-ranked players and their progress.
void lbDraw(sf::RenderWindow& window, const sf::Font& font)
{
    const float px = -290.f, pw = 280.f, top = 40.f;
    const int MAXPTS = LEVEL_TOTAL + ACH_COUNT;
    sf::Color medal[3] = { sf::Color(255, 215, 0), sf::Color(200, 200, 215), sf::Color(205, 127, 50) };

    panelBox(window, px, top, pw, 200.f);
    panelTextCentered(window, font, "TOP 3 PLAYERS", 26, sf::Color::Yellow, px + pw / 2.f, top + 8.f);
    panelLine(window, px + 10.f, top + 44.f, pw - 20.f);

    for (int i = 0; i < 3; i++)
    {
        float ry = top + 56.f + i * 46.f;
        bool has = (i < lbCount);

        sf::CircleShape badge(13.f);
        badge.setPosition({ px + 12.f, ry + 4.f });
        badge.setFillColor(has ? medal[i] : sf::Color(70, 70, 70));
        window.draw(badge);
        panelTextCentered(window, font, std::to_string(i + 1), 18, sf::Color::Black, px + 25.f, ry + 6.f);

        if (!has)
        {
            panelText(window, font, "---", 22, sf::Color(120, 120, 120), px + 48.f, ry + 4.f);
            continue;
        }

        panelText(window, font, lbName[i], 22, i == 0 ? sf::Color(255, 215, 0) : sf::Color::White, px + 48.f, ry);

        for (int s = 0; s < MAXPTS; s++)
        {
            sf::RectangleShape seg({ 13.f, 8.f });
            seg.setPosition({ px + 48.f + s * 16.f, ry + 30.f });
            seg.setFillColor(s < lbTotal(i) ? medal[i] : sf::Color(70, 70, 70));
            window.draw(seg);
        }

        panelText(window, font, std::to_string(lbTotal(i)) + "/" + std::to_string(MAXPTS),
            20, sf::Color::White, px + pw - 64.f, ry + 2.f);
        panelText(window, font, "L" + std::to_string(lbLevels[i]) + " A" + std::to_string(lbAch[i]),
            14, sf::Color(200, 200, 200), px + pw - 62.f, ry + 26.f);
    }
}


// Draws the achievement panel, progress bar, and unlock message.
void achDraw(sf::RenderWindow& window, const sf::Font& font)
{
    const float px = -290.f, pw = 280.f, top = 270.f;
    const char* achDesc[ACH_COUNT] = { "Reach 500 points", "Reach 1500 points", "Reach 3000 points",
                                       "Eat a ghost (red orb)", "Clear a level < 2 min" };

    int unlocked = 0;
    for (int i = 0; i < ACH_COUNT; i++)
        if (achDone[i]) unlocked++;

    panelBox(window, px, top, pw, 330.f);
    panelTextCentered(window, font, "ACHIEVEMENTS", 26, sf::Color::Yellow, px + pw / 2.f, top + 8.f);
    panelLine(window, px + 10.f, top + 44.f, pw - 20.f);

    panelText(window, font, "PLAYER: " + achPlayer, 18, sf::Color::White, px + 12.f, top + 52.f);
    panelText(window, font, std::to_string(unlocked) + "/" + std::to_string(ACH_COUNT),
        18, sf::Color(255, 215, 0), px + pw - 50.f, top + 52.f);

    sf::RectangleShape barBack({ pw - 24.f, 8.f });
    barBack.setPosition({ px + 12.f, top + 80.f });
    barBack.setFillColor(sf::Color(70, 70, 70));
    window.draw(barBack);
    sf::RectangleShape barFill({ (pw - 24.f) * unlocked / ACH_COUNT, 8.f });
    barFill.setPosition({ px + 12.f, top + 80.f });
    barFill.setFillColor(sf::Color(255, 215, 0));
    window.draw(barFill);

    for (int i = 0; i < ACH_COUNT; i++)
    {
        float ry = top + 100.f + i * 44.f;
        bool done = achDone[i];

        sf::RectangleShape check({ 20.f, 20.f });
        check.setPosition({ px + 14.f, ry + 4.f });
        check.setFillColor(done ? sf::Color(255, 215, 0) : sf::Color(0, 0, 0, 0));
        check.setOutlineThickness(2.f);
        check.setOutlineColor(done ? sf::Color(255, 215, 0) : sf::Color(150, 150, 150));
        window.draw(check);
        if (done)
            panelTextCentered(window, font, "X", 16, sf::Color::Black, px + 24.f, ry + 5.f);

        panelText(window, font, achName[i], 20,
            done ? sf::Color(255, 215, 0) : sf::Color(170, 170, 170), px + 44.f, ry);
        panelText(window, font, achDesc[i], 14,
            done ? sf::Color(220, 220, 220) : sf::Color(120, 120, 120), px + 44.f, ry + 24.f);
    }

    if (achPopupTimer > 0)   // "UNLOCKED" popup maze ke upar
    {
        sf::Text pop(font, achPopupText, 36);
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
bool isWall(float x, float y)
{
    if (x < 0 || y < 0)
        return true;

    int col = int(x) / CELL;
    int row = int(y) / CELL;

    if (col >= COLS || row >= ROWS)
        return true;

    return maze[row * COLS + col] == 1;
}

// Checks whether a row and column point to a wall tile.
bool isWall2(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return false;
    return maze[row * COLS + col] == 1;
}

// Checks all four corners before allowing the player to move.
bool canMoveTo(float x, float y, float half)
{
    if (isWall(x - half, y - half))
        return false;
    if (isWall(x + half, y - half))
        return false;
    if (isWall(x - half, y + half))
        return false;
    if (isWall(x + half, y + half))
        return false;
    return true;
}

// Returns true if Pac-Man and a ghost share a clear row or column.
bool ghostSharesClearLane(sf::Vector2f pacmanPos, sf::Vector2f ghostPos)
{
    const float alignmentTolerance = 3.f;

    if (pacmanPos.x < 0.f || pacmanPos.y < 0.f ||
        ghostPos.x < 0.f || ghostPos.y < 0.f ||
        pacmanPos.x >= COLS * CELL || ghostPos.x >= COLS * CELL ||
        pacmanPos.y >= ROWS * CELL || ghostPos.y >= ROWS * CELL)
        return false;

    int pacRow = static_cast<int>(pacmanPos.y) / CELL;
    int pacCol = static_cast<int>(pacmanPos.x) / CELL;
    int ghostRow = static_cast<int>(ghostPos.y) / CELL;
    int ghostCol = static_cast<int>(ghostPos.x) / CELL;

    // Same horizontal lane.
    if (pacRow == ghostRow && std::abs(pacmanPos.y - ghostPos.y) <= alignmentTolerance)
    {
        int firstCol = std::min(pacCol, ghostCol) + 1;
        int lastCol = std::max(pacCol, ghostCol);

        for (int col = firstCol; col < lastCol; col++)
        {
            if (maze[pacRow * COLS + col] == 1)
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
            if (maze[row * COLS + pacCol] == 1)
                return false;
        }
        return true;
    }

    return false;
}
//leaderboard code by hmza:

std::string topNames[6];    // player name of each run
int topScores[6];           // 5 best scores + 1 spare slot
int topTimes[6];            // seconds each run lasted
std::time_t topStamps[6];   // when each run ended (unix time)
int savedCount = 0;         // how many scores we have right now

// Changes a saved time into readable date and time text.
std::string formatStamp(std::time_t t)
{
    std::tm tmv = {};
#ifdef _WIN32
    localtime_s(&tmv, &t);
#else
    localtime_r(&t, &tmv);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%d/%m %H:%M", &tmv);
    return std::string(buf);
}

// Loads saved scores, run times, and dates when the game starts.
void loadScores()
{
    std::ifstream file("leaderboard_v2.txt");
    savedCount = 0;
    long long stamp = 0;
    while (savedCount < 5 && file >> topNames[savedCount] >> topScores[savedCount]
        >> topTimes[savedCount] >> stamp)
    {
        topStamps[savedCount] = static_cast<std::time_t>(stamp);
        savedCount++;
    }
}

// Adds a score, sorts the list, keeps the top five, and saves it.
void addScore(const std::string& name, int score, int seconds)
{
    topNames[savedCount] = name;         // put the new one at the end
    topScores[savedCount] = score;
    topTimes[savedCount] = seconds;
    topStamps[savedCount] = std::time(nullptr);   // time stamp of this run
    savedCount++;

    // sort: biggest score first (bubble sort)
    for (int i = 0; i < savedCount - 1; i++)
    {
        for (int j = 0; j < savedCount - 1 - i; j++)
        {
            if (topScores[j] < topScores[j + 1])
            {
                int temp = topScores[j];
                topScores[j] = topScores[j + 1];
                topScores[j + 1] = temp;

                temp = topTimes[j];
                topTimes[j] = topTimes[j + 1];
                topTimes[j + 1] = temp;

                std::string tn = topNames[j];
                topNames[j] = topNames[j + 1];
                topNames[j + 1] = tn;

                std::time_t ts = topStamps[j];
                topStamps[j] = topStamps[j + 1];
                topStamps[j + 1] = ts;
            }
        }
    }

    if (savedCount > 5)
        savedCount = 5;                  // forget the 6th

    // save to the file
    std::ofstream file("leaderboard_v2.txt");
    for (int i = 0; i < savedCount; i++)
        file << topNames[i] << " " << topScores[i] << " " << topTimes[i] << " "
        << static_cast<long long>(topStamps[i]) << "\n";
}

// Draws the saved high scores in the center of the screen.
void drawLeaderboard(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title(font, "TOP 5 SCORES", 40);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin({ tb.position.x + tb.size.x / 2.f, 0.f });   // horizontal centre
    title.setPosition({ 600.f, 460.f });
    window.draw(title);

    // one row of text: rank, player name, score, run time, date + time of the run
    auto makeLine = [&](int i)
        {
            return std::to_string(i + 1) + ".  " + topNames[i] + "  " +
                std::to_string(topScores[i]) + " pts  " +
                std::to_string(topTimes[i]) + " sec  " +
                formatStamp(topStamps[i]);
        };

    // find the widest row so the whole list is centred as one block
    float maxW = 0.f;
    for (int i = 0; i < savedCount; i++)
    {
        sf::Text row(font, makeLine(i), 26);
        float w = row.getLocalBounds().size.x;
        if (w > maxW)
            maxW = w;
    }

    for (int i = 0; i < savedCount; i++)
    {
        sf::Text row(font, makeLine(i), 26);
        row.setPosition({ 600.f - maxW / 2.f, 520.f + i * 40.f });
        window.draw(row);
    }
}

// Sets up the game and runs its menus, gameplay, and drawing.
int main()
{
    // seed the random numbers (ghost decisions, orb spawning)
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // window
    sf::RenderWindow window(sf::VideoMode({ 1500, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);
    // added by abd for left achievement board
    sf::View gameView(sf::FloatRect({ -300.f, 0.f }, { 1500.f, 900.f }));   // left panel ke liye 300px extra
    window.setView(gameView);

    // maze background
    sf::Texture BackGround("maze.bmp");
    sf::Sprite BackGround_S(BackGround);

    // player
    sf::Texture Player1_texA("Player1_TextureA.png");
    sf::Texture Player1_texB("Player1_TextureB.png");
    Player Player1(Player1_texA);
    Player1.sprite.setScale(
        {
            40 / float(Player1_texA.getSize().x),
            40 / float(Player1_texA.getSize().y)
        });
    float player1_width = Player1.sprite.getLocalBounds().size.x;
    float player1_height = Player1.sprite.getLocalBounds().size.y;
    Player1.sprite.setOrigin({ player1_width / 2, player1_height / 2 });
    Player1.sprite.setPosition({ 54, 846 });

    // this code is for making the ghost
        // this code is for making red ghost blinky
    sf::Texture Blinky_tex("red_ghost.png");
    Ghost Blinky(Blinky_tex);
    Blinky.sprite.setScale({
        30 / float(Blinky_tex.getSize().x),
        30 / float(Blinky_tex.getSize().y)
        });
    Blinky.sprite.setPosition({ 400,400 });
    //i,hamza writing this code to add blue ghosts in the game.

    sf::Texture blue_tex("bluee.png");
    Ghost blue(blue_tex);
    blue.sprite.setScale({
        36 / float(blue_tex.getSize().x),
        36 / float(blue_tex.getSize().y)
        });
    blue.sprite.setPosition({ 400, 360 });

    //i,hamza writing this code to add yellow ghosts in the game.

    sf::Texture Yellow_tex("yellow_ghost.png");
    Ghost Yellow(Yellow_tex);
    Yellow.sprite.setScale({
        30 / float(Yellow_tex.getSize().x),
        30 / float(Yellow_tex.getSize().y)
        });
    Yellow.sprite.setPosition({ 470, 400 });

    // this code is for making pink ghost pinky.
    sf::Texture Pinky_tex("pink_ghost.png");
    Ghost Pinky(Pinky_tex);
    Pinky.sprite.setScale({
        36 / float(Pinky_tex.getSize().x),
        36 / float(Pinky_tex.getSize().y)
        });
    Pinky.sprite.setPosition({ 470, 360 });
    // adding new ghosts for next levels by abd
    Ghost Extra1(Blinky_tex), Extra2(Pinky_tex), Extra3(blue_tex), Extra4(Yellow_tex);

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
    sf::Texture Game_Banner("Game_banner.jpg");
    sf::Sprite SideScreen(Game_Banner);
    SideScreen.setPosition({ 900,0 });
    SideScreen.setScale({ 300.f / Game_Banner.getSize().x, 900.f / Game_Banner.getSize().y });
    // abdullahameer added achievement board here
    sf::Sprite LeftScreen(Game_Banner);
    LeftScreen.setPosition({ -300.f, 0.f });
    LeftScreen.setScale({ 300.f / Game_Banner.getSize().x, 900.f / Game_Banner.getSize().y });
    sf::RectangleShape leftDark({ 300.f, 900.f });          // text saaf parhne ke liye halka andhera
    leftDark.setPosition({ -300.f, 0.f });
    leftDark.setFillColor(sf::Color(0, 0, 0, 170));
    //
    sf::Text Game_Title(font, "PACMAN", 100);
    Game_Title.setFillColor(sf::Color::Yellow);
    Game_Title.setPosition({ 910, 200 });

    sf::Text Score_counter(font, "Score: ", 50);
    Score_counter.setFillColor(sf::Color::Red);
    Score_counter.setPosition({ 950,300 });

    sf::Text Score_value(font, "0", 50);
    Score_value.setFillColor(sf::Color::Red);
    Score_value.setPosition({ 1080, 300 });

    sf::Text Lives_counter(font, "Lives Left: ", 50);
    Lives_counter.setFillColor(sf::Color::Red);
    Lives_counter.setPosition({ 920, 350 });

    sf::Text Lives_value(font, "0", 50);
    Lives_value.setFillColor(sf::Color::Red);
    Lives_value.setPosition({ 1130,350 });


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
    float half = 15;         // half of Pac-Man's size (40 / 2) //another change here half=15 from now to make pac-man movement checking defined.
    char direction = ' ';
    char wanted = ' ';       // direction the player asked for (turn happens at the next tile centre)
    bool has_the_run_started = false, current_texture = false;
    bool is_game_paused = false;

    //seconds counter for leaderboard: 

    float run_time = 0;           // seconds played in this run
    bool score_saved = false;     // so we save only once per game over
    loadScores();                 // read the file when the game starts

    // frenzy mode (5.5) by hmza
    bool frenzyOn = false;      // true while frenzy is running
    float frenzyWait = 0;       // seconds played since the last frenzy
    float frenzyLeft = 0;       // seconds of frenzy still left
    // places dots in every 1x1 area
    //Pinky.alive = blue.alive = false;   // TEST: 2 ghosts pehle se mare hue   // testing for boss
    place_dots();

    // ===== GHOST MOVEMENT SETUP =====
    // Done by MUHAMMAD KHIZAR REHMAN
    // edit by abd
    Ghost* gh[8] = { &Blinky, &Pinky, &blue, &Yellow, &Extra1, &Extra2, &Extra3, &Extra4 };
    // edited by abd for makking ghost color same for some time 
    sf::Texture* ghostTex[8] = { &Blinky_tex, &Pinky_tex, &blue_tex, &Yellow_tex, &Blinky_tex, &Pinky_tex, &blue_tex, &Yellow_tex };
    float ghostSize[8] = { 30, 36, 36, 30, 30, 36, 36, 30 }; // har ghost ka size (jo upar setScale mein diya tha) aur next levels ke liye bhi ghosts
    bool ghostsScared = false;
    // edit by abd
    int gr[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile the ghost is on
    int gc[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // column of that tile
    int nr[8] = { 11, 10, 10, 11 ,11, 10, 10, 11 };   // row of the tile it is walking to
    int nc[8] = { 11, 13, 11, 13 ,11, 13, 11, 13 };   // column of that tile
    int vr[8] = { 0, 0, 0, 0,0, 0, 0, 0 };       // last step taken (row)
    int vc[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };       // last step taken (column)
    int cr[8] = { 1, 1, 23, 23,1, 1, 23, 23 };     // corner each ghost runs to (row)
    int cc[8] = { 23, 1, 23, 1, 23, 1, 23, 1 };     // corner each ghost runs to (column)
    int dR[8] = { -1, 0, 1, 0,-1, 0, 1, 0 };      // the 4 ways: up, left, down, right
    int dC[8] = { 0, -1, 0, 1, 0, -1, 0, 1 };
    float rel[8] = { 0, 3, 6, 9, 12, 15, 18, 21 };    // Done by MUHAMMAD KHIZAR REHMAN: every ghost has its own release time
    float gt = 0, mt = 0;             // game time, mode timer
    bool scat = false;                // false = chase, true = run to corner

    // ===== LIFE LOSS SETUP (ghost touches pac-man) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool rst = false;                 // true = put ghosts back in the house
    int hr[8] = { 11, 10, 10, 11,11, 10, 10, 11 };   // home tile of each ghost (row)
    int hc[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // home tile of each ghost (column)

    // ===== GATES SETUP (5.6) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    int er = 23, ec = 1;       // entry gate tile (lower left corner)
    int xr = 1, xc = 23;       // exit gate tile (upper right corner)
    int got = 0;               // pellets eaten so far this level
    int tot = 0;               // pellets that were on the stage at the start
    for (int i = 0; i < COLS * ROWS; i++)
        if (maze[i] == 2)
            tot++;
    float rs[COLS * ROWS] = { 0 };   // respawn countdown of each eaten pellet
    float rsT = 300.f;               // an eaten pellet comes back after 300 sec (5 min)
    sf::RectangleShape gsh({ 36.f, 36.f });
    gsh.setOutlineThickness(3.f);
    gsh.setOutlineColor(sf::Color::White);
    sf::Text gtx(font, "", 25);
    gtx.setFillColor(sf::Color::White);
    gtx.setPosition({ 920.f, 600.f });

    // ===== GRAPE SETUP (5.10 extra life) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool gv = false;        // is the grape on the map right now?
    float gtm = 0;          // seconds passed since the last grape spawn
    float gsp = 300;        // spawn every 300 sec (5 min)
    sf::Texture grape_tex("grape.png");         // grape picture (hmza: image instead of the purple circle)
    sf::Sprite grp(grape_tex);
    grp.setScale({ 30.f / grape_tex.getSize().x, 30.f / grape_tex.getSize().y });   // 30 x 30 pixels on screen
    grp.setOrigin({ grape_tex.getSize().x / 2.f, grape_tex.getSize().y / 2.f });    // position = middle of the grape
    grp.setPosition({ 450.f, 306.f });          // square just outside the ghost house door

    // ===== SPECIAL BONUS ORB SETUP (5.2) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    int br = 8, bc = 11;       // bonus orb tile: just outside the ghost house door, right next to the grape
    bool bo = true;            // is the bonus orb on the map right now?
    float bw = 0;              // seconds passed since the bonus orb was eaten
    float bsp = 120;           // it comes back 120 sec (2 min) after it was eaten
    sf::CircleShape bsh(12.f);
    bsh.setFillColor(sf::Color(255, 120, 0));        // orange
    bsh.setOutlineThickness(2.f);
    bsh.setOutlineColor(sf::Color::White);
    bsh.setOrigin({ 12.f, 12.f });                   // position = middle of the orb
    bsh.setPosition({ (bc + 0.5f) * CELL, (br + 0.5f) * CELL });

    // edit by abd
    for (int i = 0; i < 8; i++)
    {
        gh[i]->sprite.setTexture(*ghostTex[i], true);
        gh[i]->sprite.setScale({ ghostSize[i] / float(ghostTex[i]->getSize().x), ghostSize[i] / float(ghostTex[i]->getSize().y) });
        gh[i]->sprite.setOrigin({ ghostTex[i]->getSize().x / 2.f, ghostTex[i]->getSize().y / 2.f });
        gh[i]->sprite.setPosition({ (gc[i] + 0.5f) * CELL, (gr[i] + 0.5f) * CELL });
    }

    // ===== END OF GHOST MOVEMENT SETUP =====


    // this makes sure that holding esc dosent make pause screen flicker
    window.setKeyRepeatEnabled(false);
    //abdullahameer coded here
        // ===== PLAYER NAME SCREEN (Abdullah Ameer) =====
    std::string playerName = "";
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

    const char* laneSoundFiles[4] = {
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

    const char* deathSoundFiles[3] = {
        "ghost3.mp3.mpeg", "cid-acp-pradhuman-fasi.mp3",
        "cid-ke-bakchodi.mp3"
    };
    for (int i = 0; i < 3; i++)
        if (!deathSoundLoaded[i])
            std::cerr << "Could not load " << deathSoundFiles[i]
            << ". Check the file and its path.\n";

    // Each event advances its own playlist independently. Unavailable files
    // are skipped, while successfully loaded files retain the requested order.
    int nextLaneSound = 0;
    int nextDeathSound = 0;
    auto playNextSound = [](sf::Sound* sounds[], const bool loaded[],
        int soundCount, int& nextIndex)
        {
            for (int attempt = 0; attempt < soundCount; attempt++)
            {
                int index = nextIndex;
                nextIndex = (nextIndex + 1) % soundCount;

                if (!loaded[index])
                    continue;

                // Keep sounds within the same playlist from overlapping.
                for (int i = 0; i < soundCount; i++)
                    sounds[i]->stop();

                sounds[index]->play();
                return;
            }
        };

    // Prevent the lane warning from restarting every frame while a ghost
    // remains in the same unobstructed row or column as Pac-Man.
    bool ghostLaneWasActive = false;

    // Do not start the pellet sound while another audio track/effect is playing.
    // This includes menu music, chase warnings, and death sounds.
    auto anotherAudioIsPlaying = [&]()
        {
            if (menuMusic.getStatus() == sf::SoundSource::Status::Playing)
                return true;

            for (int i = 0; i < 4; i++)
                if (laneWarningSounds[i]->getStatus() == sf::SoundSource::Status::Playing)
                    return true;

            for (int i = 0; i < 3; i++)
                if (deathSounds[i]->getStatus() == sf::SoundSource::Status::Playing)
                    return true;

            return false;
        };
    // ===== END AUDIO SETUP =====

    // Let the player enter a name before starting the game.
    while (window.isOpen() && !nameEntered)
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* te = event->getIf<sf::Event::TextEntered>())
            {
                char c = (te->unicode < 128) ? static_cast<char>(te->unicode) : 0;
                if (te->unicode == 8)
                {
                    if (!playerName.empty()) playerName.pop_back();
                }
                else if (playerName.size() < 10 &&
                    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
                {
                    playerName += c;
                }
            }

            if (const auto* ke = event->getIf<sf::Event::KeyPressed>())
            {
                if (ke->code == sf::Keyboard::Key::Enter && !playerName.empty())
                    nameEntered = true;
            }
        }

        nameText.setString(playerName + "_");
        window.clear();
        window.draw(nameTitle);
        window.draw(nameText);
        window.draw(nameHint);
        window.display();
    }

    // The menu/name-entry music stops as soon as the game starts.
    menuMusic.stop();

    if (playerName.empty()) playerName = "PLAYER";
    achLoad(playerName);
    lbLoad();
    lbUpdate(achPlayer, 0);
    clock.restart();
    // ===== END OF PLAYER NAME SCREEN =====

    // Keep handling game input, movement, scoring, and drawing until the window closes.
    while (window.isOpen())
    {
        // This makes sure you can pause game or exit window
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
                    is_game_paused = !is_game_paused;
            }
        }

        // first check if player has enough lives, if he does run game else show Game over screen
        if (Player1.lives > 0)
        {
            // check is game paused or not
            if (!is_game_paused)
            {
                // this variable stores the time between 2 movement key presses
                float dt = clock.restart().asSeconds();
                if (dt > 0.05f)
                    dt = 0.05f;   // Done by MUHAMMAD KHIZAR REHMAN: stops the big jump after pause

                // this variable stores the total time of the game run: (leaderboard code by hmza):
                run_time += dt;

                // frenzy mode (5.5) by hmza
                if (has_the_run_started)
                {
                    if (frenzyOn)
                    {
                        frenzyLeft -= dt;
                        if (frenzyLeft <= 0)      // 10 seconds over
                        {
                            frenzyOn = false;
                            frenzyWait = 0;       // start counting the next minute
                        }
                    }
                    else
                    {
                        frenzyWait += dt;
                        if (frenzyWait >= 60)     // 1 minute passed
                        {
                            frenzyOn = true;
                            frenzyLeft = 10;
                        }
                    }
                }
                int mult = frenzyOn ? 5 : 1;      // points multiplier
                // editimg by abd
                orbsUpdate(dt);
                //abd
                achUpdate(dt);
                if (levelBanner > 0)
                    levelBanner -= dt;

                // ===== GRID-LOCKED MOVEMENT (pac-man moves only along a single row / column) =====
                // keys only set the WANTED direction
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    wanted = 'U';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    wanted = 'L';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    wanted = 'D';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    wanted = 'R';

                sf::Vector2f pos = Player1.sprite.getPosition();
                float step = Player1.speed * dt * orbSpeedMultiplier();
                int row = int(pos.y) / CELL;
                int col = int(pos.x) / CELL;
                float cx = (col + 0.5f) * CELL;   // centre of current tile
                float cy = (row + 0.5f) * CELL;

                // turn only if that way is open and pac-man is at the tile centre (reversing is always allowed)
                if (wanted != ' ' && wanted != direction)
                {
                    bool reverse = (direction == 'U' && wanted == 'D') || (direction == 'D' && wanted == 'U') ||
                        (direction == 'L' && wanted == 'R') || (direction == 'R' && wanted == 'L');
                    int tr = row + (wanted == 'D') - (wanted == 'U');
                    int tc = col + (wanted == 'R') - (wanted == 'L');
                    bool atCentre = std::abs(pos.x - cx) <= step && std::abs(pos.y - cy) <= step;

                    if (!isWall2(tr, tc) && (reverse || atCentre))
                    {
                        if (!reverse)
                            pos = { cx, cy };          // snap into the new lane
                        direction = wanted;
                        has_the_run_started = true;
                    }
                }

                // move along the current lane only
                float newX = pos.x, newY = pos.y;
                bool horizontal = (direction == 'L' || direction == 'R');

                if (direction == 'U') { Player1.sprite.setRotation(sf::degrees(270)); newY -= step; }
                else if (direction == 'L') { Player1.sprite.setRotation(sf::degrees(180)); newX -= step; }
                else if (direction == 'D') { Player1.sprite.setRotation(sf::degrees(90));  newY += step; }
                else if (direction == 'R') { Player1.sprite.setRotation(sf::degrees(360)); newX += step; }

                if (direction != ' ')
                {
                    if (canMoveTo(newX, newY, half))
                        pos = { newX, newY };
                    else if (horizontal)
                        pos.x = cx;                    // hit a wall: stop at tile centre
                    else
                        pos.y = cy;

                    if (horizontal) pos.y = cy;        // lock to the row
                    else            pos.x = cx;        // lock to the column
                }

                // starts the pacman animation if any movement key is pressed
                if (has_the_run_started)
                {
                    // write code for changing of texture
                    if (timer_for_texture_change.getElapsedTime().asSeconds() >= 0.3)
                    {
                        current_texture = !current_texture;
                        if (current_texture)
                            Player1.sprite.setTexture(Player1_texA);
                        else
                            Player1.sprite.setTexture(Player1_texB);
                        timer_for_texture_change.restart();
                    }
                }

                Player1.sprite.setPosition(pos);
                // ===== END GRID-LOCKED MOVEMENT =====


                // Check every maze tile for pellets close enough to collect.
                for (int r = 0; r < ROWS; r++)
                {
                    for (int c = 0; c < COLS; c++)
                    {
                        if (maze[r * COLS + c] == 2)
                        {
                            float dx = pos.x - (c + 0.5f) * CELL;
                            float dy = pos.y - (r + 0.5f) * CELL;
                            if (dx * dx + dy * dy < 15.f * 15.f)
                            {
                                maze[r * COLS + c] = 0;
                                if (pelletSoundLoaded && !anotherAudioIsPlaying())
                                {
                                    // Pellet audio is only allowed when no other
                                    // audio is playing. It is kept at 50% volume.
                                    pelletSound.stop();
                                    pelletSound.play();
                                }
                                Player1.score += 10 * mult;
                                got++;                        // Done by MUHAMMAD KHIZAR REHMAN: count this pellet
                                rs[r * COLS + c] = rsT;       // Done by MUHAMMAD KHIZAR REHMAN: it will come back later
                            }
                        }
                    }
                }
                // orbits  and levels speed added by abdullahameer
                Player1.score += orbsPickup(pos) * mult;
                achCheckScore(Player1.score);

                // ===== PELLET RESPAWN + EXIT GATE (5.6) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                for (int k = 0; k < COLS * ROWS; k++)
                {
                    if (rs[k] > 0)
                    {
                        rs[k] -= dt;
                        if (rs[k] <= 0 && maze[k] == 0)
                            maze[k] = 2;                  // the pellet is back
                    }
                }
                float qx = pos.x - (xc + 0.5f) * CELL;
                float qy = pos.y - (xr + 0.5f) * CELL;

                // ===== LEVEL UP (6.1, 6.2) =====
                // Done by MUHAMMAD KHIZAR REHMAN: the level is now cleared at the open exit gate (was: dotsLeft == 0)
                if (got >= tot && qx * qx + qy * qy < 18.f * 18.f)
                {
                    level++;
                    // edited by abd
                    achLevelCleared(run_time - levelStartTime);
                    levelStartTime = run_time;
                    lbUpdate(achPlayer, level - 1);   // levels cleared so far
                    //done
                    levelSpeed *= 1.10f;                        // ghosts aur Pacman dono tez
                    if (ghostCount < MAX_GHOSTS)
                        ghostCount++;  // 1 ghost zyada
                    levelBanner = 2.5f;                         // "LEVEL n" 2.5 sec dikhao

                    place_dots();
                    Player1.score -= 10;                        // start tile ka dot dobara khaya jata hai
                    Player1.sprite.setPosition({ 54, 846 });
                    pos = Player1.sprite.getPosition();
                    direction = ' ';
                    wanted = ' ';
                    orbsReset(true);
                    rst = true;                                 // ghosts wapas house mein

                    got = 0;                                    // Done by MUHAMMAD KHIZAR REHMAN: new level, new pellet count
                    for (int k = 0; k < COLS * ROWS; k++)
                        rs[k] = 0;

                    bo = true;                                  // Done by MUHAMMAD KHIZAR REHMAN: bonus orb is back on a new level
                    bw = 0;

                    for (int i = 0; i < 8; i++)                 // sab ghosts zinda aur normal
                    {
                        gh[i]->alive = true;
                        gh[i]->isBoss = false;
                        gh[i]->speed = 130;
                        gh[i]->sprite.setColor(sf::Color::White);
                        gh[i]->sprite.setTexture(*ghostTex[i], true);
                        gh[i]->sprite.setScale({ ghostSize[i] / ghostTex[i]->getSize().x, ghostSize[i] / ghostTex[i]->getSize().y });
                        gh[i]->sprite.setOrigin({ ghostTex[i]->getSize().x / 2.f, ghostTex[i]->getSize().y / 2.f });
                    }
                    ghostsScared = false;
                }
                // ===== END LEVEL UP =====

                // kill-ghost orb chal raha ho to sab ghosts ek jese, khatam ho to wapas asli
                bool scaredNow = orbKillActive();
                if (scaredNow != ghostsScared)
                {
                    ghostsScared = scaredNow;
                    for (int i = 0; i < 8; i++)
                    {
                        if (gh[i]->isBoss)
                            continue;                 // boss keeps its red boss look
                        sf::Texture& t = ghostsScared ? blue_tex : *ghostTex[i];
                        gh[i]->sprite.setTexture(t, true);
                        gh[i]->sprite.setScale({ ghostSize[i] / float(t.getSize().x),
                                                 ghostSize[i] / float(t.getSize().y) });
                        gh[i]->sprite.setOrigin({ t.getSize().x / 2.f, t.getSize().y / 2.f });
                    }
                }



                // Move each active ghost toward its next target tile.
                // ===== GHOST MOVEMENT =====
                // Done by MUHAMMAD KHIZAR REHMAN

                // after a hit, put the ghosts back in the house and restart their timers
                if (rst)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        gr[i] = nr[i] = hr[i];
                        gc[i] = nc[i] = hc[i];
                        vr[i] = 0;
                        vc[i] = 0;
                        gh[i]->sprite.setPosition({ (hc[i] + 0.5f) * CELL, (hr[i] + 0.5f) * CELL });
                    }
                    // edited by abdullahameer
                    // Done by MUHAMMAD KHIZAR REHMAN: reset the release time of ALL 8 ghosts
                    for (int i = 0; i < 8; i++)
                        rel[i] = i * 3.f;
                    // done here
                    gt = 0;
                    mt = 0;
                    scat = false;
                    has_the_run_started = false;
                    rst = false;
                }

                if (has_the_run_started)
                {
                    gt += dt * (frenzyOn ? 2.f : 1.f);   // frenzy: ghosts leave the house twice as fast
                    mt += dt;
                }
                if (scat && mt >= 7)
                {
                    scat = false; mt = 0;
                }        // 7 sec corner run
                else if (!scat && mt >= 20)
                {
                    scat = true; mt = 0;
                }  // then 20 sec chase

         // pac-man's tile and which way he faces
                int pr = int(pos.y) / CELL;
                int pc = int(pos.x) / CELL;
                int ar = 0, ac = 0;
                if (direction == 'U')
                    ar = -1;
                else if
                    (direction == 'D') ar = 1;
                else if
                    (direction == 'L') ac = -1;
                else if
                    (direction == 'R') ac = 1;

                for (int i = 0; i < ghostCount; i++)
                {
                    if (!has_the_run_started || gt < rel[i] || !gh[i]->alive)
                        continue;

                    sf::Vector2f gp = gh[i]->sprite.getPosition();
                    float tx = (nc[i] + 0.5f) * CELL;
                    float ty = (nr[i] + 0.5f) * CELL;
                    float step = gh[i]->speed * dt * levelSpeed;
                    float left = std::abs(tx - gp.x) + std::abs(ty - gp.y);

                    // still walking to the next tile
                    if (left > step)
                    {
                        gp.x += vc[i] * step;
                        gp.y += vr[i] * step;
                        gh[i]->sprite.setPosition(gp);
                        continue;
                    }

                    // reached the tile centre, so snap there and decide the next turn
                    gh[i]->sprite.setPosition({ tx, ty });
                    gr[i] = nr[i];
                    gc[i] = nc[i];
                    // making movements like other ghosts
                    int kind = i % 4;      // naye ghosts purane 4 ki tarah chalein
                    // 1) choose the target square
                    int tr, tc;
                    bool inH = (gr[i] >= 9 && gr[i] <= 11 && gc[i] >= 11 && gc[i] <= 13);
                    if (inH)
                    {
                        tr = 8; tc = 12;                   // head for the house door
                    }
                    else if (gh[i]->isBoss)
                    {
                        tr = pr; tc = pc;                  // boss always chases pac-man
                    }
                    else if (scat)
                    {
                        tr = cr[i]; tc = cc[i];            // own corner
                    }
                    else if (kind == 0)
                    {
                        tr = pr; tc = pc;                  // red: pac-man himself
                    }
                    else if (kind == 1)
                    {
                        tr = pr + 4 * ar;                  // pink: 4 squares ahead
                        tc = pc + 4 * ac;
                    }
                    else if (kind == 2)
                    {
                        int mr = pr + 2 * ar, mc = pc + 2 * ac;  // 2 squares ahead
                        tr = 2 * mr - gr[0];                     // red -> there, doubled
                        tc = 2 * mc - gc[0];
                    }
                    else
                    {
                        int da = pr - gr[i], db = pc - gc[i];
                        if (da * da + db * db < 64)        // closer than 8 squares
                        {
                            tr = cr[i]; tc = cc[i];        // run home
                        }
                        else
                        {
                            tr = pr; tc = pc;              // far away, so chase
                        }
                    }

                    // 2) of the 4 ways, pick the one that ends closest to the target
                    //    (random: ties are broken randomly)
                    int best = -1, bd = 999999;
                    int validDir[4], validCount = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        if (dR[k] == -vr[i] && dC[k] == -vc[i])
                            continue;                                   // no turning back
                        int r2 = gr[i] + dR[k], c2 = gc[i] + dC[k];
                        if (r2 < 0 || r2 >= ROWS || c2 < 0 || c2 >= COLS)
                            continue;
                        if (maze[r2 * COLS + c2] == 1 || maze[r2 * COLS + c2] == -1)
                            continue;                                   // wall (or tunnel edge)
                        if (!inH && r2 >= 9 && r2 <= 11 && c2 >= 11 && c2 <= 13)
                            continue;                                   // can't go back in the house
                        validDir[validCount++] = k;
                        int d = (r2 - tr) * (r2 - tr) + (c2 - tc) * (c2 - tc);
                        if (d < bd || (d == bd && std::rand() % 2 == 0))
                        {
                            bd = d;
                            best = k;
                        }
                    }

                    // random ghost decision: 8% of the time a normal ghost (not the boss,
                    // not leaving the house) takes a random open way at a junction
                    if (validCount > 1 && !inH && !gh[i]->isBoss && std::rand() % 100 < 8)
                        best = validDir[std::rand() % validCount];

                    if (best == -1)              // dead end: the only time it may turn back
                    {
                        vr[i] = -vr[i];
                        vc[i] = -vc[i];
                    }
                    else
                    {
                        vr[i] = dR[best];
                        vc[i] = dC[best];
                    }
                    nr[i] = gr[i] + vr[i];
                    nc[i] = gc[i] + vc[i];
                }
                // boss mode add by abd inwhich when two ghosts are kill so other 2 ghosts turn into boss mode
                // ===== BOSS MODE (5.8) =====
                int aliveCount = 0;
                for (int i = 0; i < ghostCount; i++)
                    if (gh[i]->alive)
                        aliveCount++;

                if (aliveCount <= 1)                          // only 1 ghost left: it becomes the boss
                {
                    for (int i = 0; i < ghostCount; i++)
                    {
                        if (!gh[i]->alive || gh[i]->isBoss)
                            continue;   // mara hua ya pehle se boss: skip

                        gh[i]->isBoss = true;
                        gh[i]->speed = 165;                              // 130 x 1.6
                        sf::Texture& bt = *ghostTex[i];                  // scared texture ho to asli wapas
                        gh[i]->sprite.setTexture(bt, true);
                        gh[i]->sprite.setScale({ 40.f / bt.getSize().x, 40.f / bt.getSize().y });
                        gh[i]->sprite.setOrigin({ bt.getSize().x / 2.f, bt.getSize().y / 2.f });
                        gh[i]->sprite.setColor(sf::Color(255, 90, 90));  // laal rang
                    }
                }
                // ===== END BOSS MODE =====



                // ===== LIFE LOSS: ghost touches pac-man =====
                // Done by MUHAMMAD KHIZAR REHMAN
                for (int i = 0; i < ghostCount; i++)            // Done by MUHAMMAD KHIZAR REHMAN: all active ghosts (was i < 4)
                {
                    if (!gh[i]->alive)
                        continue;                               // Done by MUHAMMAD KHIZAR REHMAN: dead ghosts can't touch
                    sf::Vector2f q = gh[i]->sprite.getPosition();
                    float ux = q.x - pos.x, uy = q.y - pos.y;


                    // Edited by abdullahameer for orbit speed
                    if (ux * ux + uy * uy < 22.f * 22.f)        // ghost is touching pac-man
                    {
                        // how many ghosts are still alive on this level
                        int aliveNow = 0;
                        for (int k = 0; k < ghostCount; k++)
                            if (gh[k]->alive)
                                aliveNow++;

                        if (orbKillActive() && !gh[i]->isBoss && aliveNow > 1)
                        {
                            // kill-ghost orb: the ghost dies until the next level (a boss ghost can't die)
                            Player1.score += 200 * mult;
                            gh[i]->alive = false;
                            // abd write this line to eat foods
                            achGhostEaten();
                        }
                        else if (!orbInvincible())
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
                            Player1.lives--;                         // lose one life
                            Player1.sprite.setPosition({ 54, 846 }); // pac-man goes back to start
                            direction = ' ';
                            wanted = ' ';
                            orbsReset(false);                        // orb ke timers band (sirf yeh ek line nayi hai)
                            rst = true;                              // ghosts go back to the house
                            break;                                   // only one life per hit
                        }
                        // agar invincible chal raha hai to kuch nahi hota
                    }
                }

                // ===== GHOST ENTERS PAC-MAN'S LANE AUDIO WARNING =====
                // Check active, living ghosts after their movement for this frame.
                bool ghostInPacmanLane = false;
                if (has_the_run_started && Player1.lives > 0)
                {
                    for (int i = 0; i < ghostCount; i++)
                    {
                        if (!gh[i]->alive || gt < rel[i])
                            continue;

                        if (ghostSharesClearLane(pos, gh[i]->sprite.getPosition()))
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
                if (has_the_run_started)
                    gtm += dt;
                if (gtm >= gsp)         // 5 minutes passed
                {
                    gv = true;          // grape appears
                    gtm = 0;
                }
                if (gv)
                {
                    float fx = pos.x - 450.f, fy = pos.y - 306.f;
                    if (fx * fx + fy * fy < 20.f * 20.f)     // pac-man touches the grape
                    {
                        Player1.lives++;                     // extra life
                        gv = false;                          // grape disappears
                    }
                }

                // ===== SPECIAL BONUS ORB (5.2) =====
                // Done by MUHAMMAD KHIZAR REHMAN
                if (!bo)
                {
                    bw += dt;               // count the seconds since it was eaten
                    if (bw >= bsp)          // 2 minutes passed
                    {
                        bo = true;          // the orb is back
                        bw = 0;
                    }
                }
                else
                {
                    float ox = pos.x - (bc + 0.5f) * CELL;
                    float oy = pos.y - (br + 0.5f) * CELL;
                    if (ox * ox + oy * oy < 18.f * 18.f)     // pac-man touches the orb
                    {
                        Player1.score += 1000 * mult;        // 1000 bonus points
                        bo = false;                          // the orb disappears
                        bw = 0;                              // the 2 minute countdown starts
                    }
                }
                // ===== END OF GHOST MOVEMENT =====

                // 3. DRAW
            }

            // Move Pac-Man to the other side when he reaches a tunnel edge.
            // working on teleportation

            if (Player1.sprite.getPosition().y >= 398 && Player1.sprite.getPosition().y <= 453)
            {
                if (Player1.sprite.getPosition().x <= 41)
                    Player1.sprite.setPosition({ 860, 414 });
                else if (Player1.sprite.getPosition().x >= 882)
                    Player1.sprite.setPosition({ 43, 414 });
            }

            // top / bottom teleport (the 3-wide gaps in the top and bottom walls, columns 11-13)
            if (Player1.sprite.getPosition().x >= 396 && Player1.sprite.getPosition().x < 504)
            {
                if (Player1.sprite.getPosition().y <= 41)
                    Player1.sprite.setPosition({ Player1.sprite.getPosition().x, 860 });
                else if (Player1.sprite.getPosition().y >= 882)
                    Player1.sprite.setPosition({ Player1.sprite.getPosition().x, 43 });
            }


            // Convert the score number to text for the sidebar.
            char buff[12], digits[12];
            int length = 0;
            int n = Player1.score, d = 0;
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
            Score_value.setString(buff);

            // Convert the lives number to text for the sidebar.
            char buff2[12], digits2[12];
            int length2 = 0;
            int n2 = Player1.lives, d2 = 0;
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
            Lives_value.setString(buff2);



            // Draw the maze, status panels, pellets, and power-ups for this frame.
            window.clear();
            BackGround_S.setColor(frenzyOn ? sf::Color(150, 90, 200) : sf::Color::White);   // tint background in frenzy
            window.draw(BackGround_S);
            window.draw(SideScreen);
            window.draw(Game_Title);
            window.draw(Score_counter);
            window.draw(Score_value);
            window.draw(Lives_counter);
            window.draw(Lives_value);
            orbsDrawHud(window, font);

            // TEST: red squares over wall cells (delete when walls are finished)
            // This is rayan, Dont delete this, i am using it to build maze

            sf::RenderStates glowStates;
            glowStates.blendMode = sf::BlendAdd;   // colors add up, which gives the glow

            // outer glow, then inner glow (additive)
            if (frenzyOn)   // frenzy colors: pink / purple
            {
                drawWallLayer(window, 30.f, sf::Color(255, 0, 200, 40), glowStates);
                drawWallLayer(window, 22.f, sf::Color(255, 40, 220, 90), glowStates);
            }
            else
            {
                drawWallLayer(window, 30.f, sf::Color(255, 180, 0, 40), glowStates);
                drawWallLayer(window, 22.f, sf::Color(255, 200, 20, 90), glowStates);
            }

            // bright thin core on top (normal drawing)
            drawWallLayer(window, 10.f, frenzyOn ? sf::Color(255, 200, 255) : sf::Color(255, 248, 190), sf::RenderStates::Default);
            //draw dots
            sf::CircleShape dot(3.f);
            dot.setFillColor(sf::Color::Yellow);

            for (int r = 0; r < ROWS; r++)
            {
                for (int c = 0; c < COLS; c++)
                {
                    if (maze[r * COLS + c] == 2)
                    {
                        dot.setPosition({ (c + 0.5f) * CELL - 3.f, (r + 0.5f) * CELL - 3.f });
                        window.draw(dot);
                    }
                }
            }

            // ===== GATES + SIDEBAR INFO (5.6) + GRAPE DRAW (5.10) =====
            // Done by MUHAMMAD KHIZAR REHMAN
            gsh.setPosition({ float(ec * CELL), float(er * CELL) });
            gsh.setFillColor(sf::Color(0, 150, 255, 140));          // entry gate: blue
            window.draw(gsh);

            gsh.setPosition({ float(xc * CELL), float(xr * CELL) });
            if (got >= tot)
                gsh.setFillColor(sf::Color(50, 255, 80, 160));      // exit open: green
            else
                gsh.setFillColor(sf::Color(255, 50, 50, 140));      // exit locked: red
            window.draw(gsh);

            gtx.setString("Level: " + std::to_string(level) + "\nPellets: " + std::to_string(got) + "/" + std::to_string(tot) + "\nEXIT: " + (got >= tot ? "OPEN" : "LOCKED"));
            window.draw(gtx);

            if (gv)
                window.draw(grp);

            if (bo)
                window.draw(bsh);   // Done by MUHAMMAD KHIZAR REHMAN: special bonus orb (5.2)

            // abdullahameer code here

            orbsDraw(window, run_time);
            window.draw(LeftScreen);
            achDraw(window, font);
            lbDraw(window, font);

            if (frenzyOn)
            {
                sf::Text frenzyText(font, "FRENZY x5: " + std::to_string(int(frenzyLeft) + 1), 30);
                frenzyText.setFillColor(sf::Color::Magenta);
                frenzyText.setPosition({ 930.f, 560.f });
                window.draw(frenzyText);
            }
        }

        window.draw(Player1.sprite);   // last, so he is on top
        if (is_game_paused)
        {
            window.draw(overlay);
            window.draw(paused_text);
            window.draw(paused_text_additional);
        }


        for (int i = 0; i < ghostCount; i++)
            if (gh[i]->alive) window.draw(gh[i]->sprite);

        if (levelBanner > 0)                                    // LEVEL n screen
        {
            sf::Text lvlText(font, "LEVEL " + std::to_string(level), 120);
            lvlText.setFillColor(sf::Color::Yellow);
            lvlText.setPosition({ 250.f, 380.f });
            window.draw(lvlText);
        }


        // Show the game-over screen and restart the game when requested.
        if (Player1.lives <= 0)
        {
            // save the score if not already done (leaderboard code by hmza):
            if (!score_saved)
            {
                addScore(playerName, Player1.score, int(run_time));
                score_saved = true;
            }

            window.draw(game_over_screen);
            window.draw(game_over_text);
            window.draw(game_over_text_additional);
            drawLeaderboard(window, font);              // shows the top 5 list with names (leaderboard code by hmza)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                Player1.lives = 1;
                Player1.score = -10;
                place_dots();
                Player1.sprite.setPosition({ 54, 846 });
                clock.restart();
                direction = ' ';
                wanted = ' ';
                run_time = 0;
                // new run starts from zero (leaderboard code by hmza)
                // edit by abd
                levelStartTime = 0;
                score_saved = false;
                frenzyOn = false;      // frenzy resets on restart (hmza)
                frenzyWait = 0;
                // allow the next game over to save again
                 // abdullahameer code
                orbsReset(true);

                // Done by MUHAMMAD KHIZAR REHMAN: gates and grape start fresh
                got = 0;
                for (int k = 0; k < COLS * ROWS; k++)
                    rs[k] = 0;
                gv = false;
                gtm = 0;
                bo = true;             // Done by MUHAMMAD KHIZAR REHMAN: bonus orb starts fresh
                bw = 0;

                // ===== LEVEL aur GHOSTS reset (abdullahameer) =====
                level = 1;
                levelSpeed = 1.f;
                ghostCount = 4;
                levelBanner = 0;

                for (int i = 0; i < 8; i++)
                {
                    gh[i]->alive = true;
                    gh[i]->isBoss = false;
                    gh[i]->speed = 130;
                    gh[i]->sprite.setColor(sf::Color::White);
                    gh[i]->sprite.setTexture(*ghostTex[i], true);
                    gh[i]->sprite.setScale({ ghostSize[i] / ghostTex[i]->getSize().x, ghostSize[i] / ghostTex[i]->getSize().y });
                    gh[i]->sprite.setOrigin({ ghostTex[i]->getSize().x / 2.f, ghostTex[i]->getSize().y / 2.f });
                }
                ghostsScared = false;
                rst = true;                    // ghosts wapas house mein
                // ===== END RESET =====
            }


        }
        window.display();

    }
}

// Fills walkable maze tiles with pellets, leaving the ghost house empty.
void place_dots()
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (maze[r * COLS + c] != 0)
                continue;

            // no dots inside the ghost house
            bool inHouse = (r >= 9 && r <= 12 && c >= 10 && c <= 14);
            if (!inHouse)
                maze[r * COLS + c] = 2;
        }
    }
}

// Draws wall tiles and joins nearby tiles into a smooth glowing layer.
void drawWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states)
{
    sf::RectangleShape piece;
    piece.setFillColor(color);

    float margin = (CELL - thickness) / 2.f;   // gap between tile edge and the shape

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (!isWall2(row, col))
                continue;

            float x = float(col * CELL);
            float y = float(row * CELL);

            // square in the middle of the tile
            piece.setSize({ thickness, thickness });
            piece.setPosition({ x + margin, y + margin });
            window.draw(piece, states);

            // connector to the wall on the right
            if (isWall2(row, col + 1))
            {
                piece.setSize({ CELL - thickness, thickness });
                piece.setPosition({ x + margin + thickness, y + margin });
                window.draw(piece, states);
            }

            // connector to the wall below
            if (isWall2(row + 1, col))
            {
                piece.setSize({ thickness, CELL - thickness });
                piece.setPosition({ x + margin, y + margin + thickness });
                window.draw(piece, states);
            }
        }
    }
}
