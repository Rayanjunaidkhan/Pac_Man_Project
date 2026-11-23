#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
# include<optional> // Abd
#include <iostream>
#include <cmath>   // Done by MUHAMMAD KHIZAR REHMAN
struct Player
{
    sf::Sprite sprite;
    int lives = 3;
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


void place_dots();
void drawWallLayer(sf::RenderWindow& window, float thickness, sf::Color color, sf::RenderStates states);

// making constants coz these are fixed mapping values of game.
const int CELL = 36;   // size of one square, in pixels
const int COLS = 25;   // how many squares across
const int ROWS = 25;   // how many squares down

int maze[COLS * ROWS] = {
1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1, // row 0
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
1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1  // row 24
};
// This is abdullah ameer code for making orbits and speeding process
// ===== ORB CODE (5.1 Multiple Power-Up Orbs) =====
const int   ORB_COUNT = 4;
const int   ORB_TILE = 36;
int  orbRow[ORB_COUNT] = { 1, 1, 13, 23 };
int  orbCol[ORB_COUNT] = { 1, 23, 12, 23 };
int  orbType[ORB_COUNT] = { 1, 2, 3, 1 };    // 1 = speed, 2 = invincible, 3 = kill-ghost
bool orbOn[ORB_COUNT] = { true, true, true, true };

const float SPEED_TIME = 5.f;
const float INVINCIBLE_TIME = 7.f;
const float KILL_TIME = 10.f;
const float SPEED_BOOST = 1.6f;

float speedTimer = 0, invTimer = 0, killTimer = 0;
// working for next levels by abd
int   level = 1;               // abhi kaun sa level hai
float levelSpeed = 1.f;        // har level par x1.10
int   ghostCount = 4;          // abhi kitne ghosts khel mein hain
const int MAX_GHOSTS = 8;
float levelBanner = 0;         // "LEVEL 2" screen ka timer
sf::Color orbColor(int type)
{
    if (type == 1)
        return sf::Color(0, 220, 255);
    if (type == 2)
        return sf::Color(255, 215, 0);
    return sf::Color(255, 40, 40);
}

void orbsUpdate(float dt)
{
    if (speedTimer > 0)
        speedTimer -= dt;
    if (invTimer > 0)
        invTimer -= dt;
    if (killTimer > 0)
        killTimer -= dt;
}
float orbSpeedMultiplier()
{
    return (speedTimer > 0 ? SPEED_BOOST : 1.f) * levelSpeed;
}
bool  orbInvincible()
{
    return invTimer > 0;
}
bool  orbKillActive()
{
    return killTimer > 0;
}

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
            if (orbType[o] == 1) speedTimer = SPEED_TIME;
            else if (orbType[o] == 2) invTimer = INVINCIBLE_TIME;
            else killTimer = KILL_TIME;
            bonus += 50;
        }
    }
    return bonus;
}

void orbsReset(bool respawnOrbs)
{
    speedTimer = 0;
    invTimer = 0;
    killTimer = 0;
    if (respawnOrbs)
        for (int o = 0; o < ORB_COUNT; o++)
            orbOn[o] = true;
}

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
    text.setFillColor(sf::Color::White);
    text.setPosition({ 930.f, 430.f });
    window.draw(text);
}
// ===== END OF ORB CODE =====


// ===== ADDING THESE TWO FUNCTIONS HERE COZ THEY ARE FOR MOVEMENT+DEFINED BOUNDARIES =====

// is wall 1 is used in making sure player cant cross walls
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

// is wall2 is used in map construction
bool isWall2(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return false;
    return maze[row * COLS + col] == 1;
}

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
//leaderboard code by hmza:

int topScores[6];      // 5 best scores + 1 spare slot
int topTimes[6];       // seconds each run lasted
int savedCount = 0;    // how many scores we have right now

// read the file when the game starts
void loadScores()
{
    std::ifstream file("leaderboard.txt");
    savedCount = 0;
    while (savedCount < 5 && file >> topScores[savedCount] >> topTimes[savedCount])
        savedCount++;
}

// add a new score, sort, keep 5, save
void addScore(int score, int seconds)
{
    topScores[savedCount] = score;       // put the new one at the end
    topTimes[savedCount] = seconds;
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
            }
        }
    }

    if (savedCount > 5)
        savedCount = 5;                  // forget the 6th

    // save to the file
    std::ofstream file("leaderboard.txt");
    for (int i = 0; i < savedCount; i++)
        file << topScores[i] << " " << topTimes[i] << "\n";
}

// show the list on the screen
void drawLeaderboard(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title(font, "TOP 5 SCORES", 40);
    title.setPosition({ 400.f, 550.f });
    window.draw(title);

    for (int i = 0; i < savedCount; i++)
    {
        std::string line = std::to_string(i + 1) + ".  " +
            std::to_string(topScores[i]) + " pts   " +
            std::to_string(topTimes[i]) + " sec";
        sf::Text row(font, line, 30);
        row.setPosition({ 400.f, 600.f + i * 40.f });
        window.draw(row);
    }
}

int main()
{
    // window
    sf::RenderWindow window(sf::VideoMode({ 1200, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);

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
    game_over_text.setPosition({ 200,300 });

    sf::Text game_over_text_additional(font, "Press Space to start again", 50);
    game_over_text_additional.setPosition({ 330, 500 });



    // variables (each declared ONCE)
    sf::Clock clock, timer_for_texture_change;
    float half = 15;         // half of Pac-Man's size (40 / 2) //another change here half=15 from now to make pac-man movement checking defined.
    char direction = ' ';
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
    float rel[8] = { 0, 3, 6, 9,0, 3, 6, 9 };    // seconds before each ghost leaves the house
    float gt = 0, mt = 0;             // game time, mode timer
    bool scat = false;                // false = chase, true = run to corner

    // ===== LIFE LOSS SETUP (ghost touches pac-man) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool rst = false;                 // true = put ghosts back in the house
    int hr[8] = { 11, 10, 10, 11,11, 10, 10, 11 };   // home tile of each ghost (row)
    int hc[8] = { 11, 13, 11, 13,11, 13, 11, 13 };   // home tile of each ghost (column)
    // ===== GRAPE SETUP (5.10 extra life) =====
    // Done by MUHAMMAD KHIZAR REHMAN
    bool gv = false;        // is the grape on the map right now?
    float gtm = 0;          // seconds passed since the last grape spawn
    float gsp = 5;        // spawn every 300 sec (5 min)
    sf::Texture grape_tex("grape.png");         // grape picture (hmza: image instead of the purple circle)
    sf::Sprite grp(grape_tex);
    grp.setScale({ 30.f / grape_tex.getSize().x, 30.f / grape_tex.getSize().y });   // 30 x 30 pixels on screen
    grp.setOrigin({ grape_tex.getSize().x / 2.f, grape_tex.getSize().y / 2.f });    // position = middle of the grape
    grp.setPosition({ 450.f, 306.f });          // square just outside the ghost house door

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
                if (levelBanner > 0)
                    levelBanner -= dt;

                // changes direction if movement key pressed
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    direction = 'U';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    direction = 'L';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    direction = 'D';
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    direction = 'R';

                // where he is now, and where he WANTS to go
                sf::Vector2f pos = Player1.sprite.getPosition();
                float newX = pos.x;
                float newY = pos.y;

                if (direction == 'U')
                {
                    Player1.sprite.setRotation(sf::degrees(270));
                    newY -= Player1.speed * dt * orbSpeedMultiplier();
                    has_the_run_started = true;
                }
                else if (direction == 'L')
                {
                    Player1.sprite.setRotation(sf::degrees(180));
                    newX -= Player1.speed * dt * orbSpeedMultiplier();
                    has_the_run_started = true;
                }
                else if (direction == 'D')
                {
                    Player1.sprite.setRotation(sf::degrees(90));
                    newY += Player1.speed * dt * orbSpeedMultiplier();
                    has_the_run_started = true;
                }
                else if (direction == 'R')
                {
                    Player1.sprite.setRotation(sf::degrees(360));
                    newX += Player1.speed * dt * orbSpeedMultiplier();
                    has_the_run_started = true;
                }

                // starts the pacman animation if any movement key is pressed
                if (has_the_run_started == true)
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
                // only move if the wanted spot is inside the limits
                if (canMoveTo(newX, pos.y, half))
                    pos.x = newX;
                if (canMoveTo(pos.x, newY, half))
                    pos.y = newY;

                Player1.sprite.setPosition(pos);


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
                                Player1.score += 10 * mult;
                            }
                        }
                    }
                }
                // orbits  and levels speed added by abdullahameer
                Player1.score += orbsPickup(pos) * mult;
                // ===== LEVEL UP (6.1, 6.2) =====
                int dotsLeft = 0;
                for (int r = 0; r < ROWS; r++)
                    for (int c = 0; c < COLS; c++)
                        if (maze[r * COLS + c] == 2) dotsLeft++;

                if (dotsLeft == 0)                              // sab dots khatam = level complete
                {
                    level++;
                    levelSpeed *= 1.10f;                        // ghosts aur Pacman dono tez
                    if (ghostCount < MAX_GHOSTS)
                        ghostCount++;  // 1 ghost zyada
                    levelBanner = 2.5f;                         // "LEVEL n" 2.5 sec dikhao

                    place_dots();
                    Player1.score -= 10;                        // start tile ka dot dobara khaya jata hai
                    Player1.sprite.setPosition({ 54, 846 });
                    pos = Player1.sprite.getPosition();
                    direction = ' ';
                    orbsReset(true);
                    rst = true;                                 // ghosts wapas house mein

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
                        sf::Texture& t = ghostsScared ? blue_tex : *ghostTex[i];
                        gh[i]->sprite.setTexture(t, true);
                        gh[i]->sprite.setScale({ ghostSize[i] / float(t.getSize().x),
                                                 ghostSize[i] / float(t.getSize().y) });
                        gh[i]->sprite.setOrigin({ t.getSize().x / 2.f, t.getSize().y / 2.f });
                    }
                }



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
                    rel[0] = 0; rel[1] = 3; rel[2] = 6; rel[3] = 9;
                    // done here
                    gt = 0;
                    mt = 0;
                    scat = false;
                    has_the_run_started = false;
                    rst = false;
                }

                if (has_the_run_started)
                {
                    gt += dt;
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
                    if (!has_the_run_started || gt < rel[i])
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
                    int best = -1, bd = 999999;
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
                        int d = (r2 - tr) * (r2 - tr) + (c2 - tc) * (c2 - tc);
                        if (d < bd)
                        {
                            bd = d;
                            best = k;
                        }
                    }

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
                for (int i = 0; i < 4; i++)
                    if (gh[i]->alive)
                        aliveCount++;

                if (aliveCount <= 2)                          // 2 ya kam ghosts bache hain
                {
                    for (int i = 0; i < 4; i++)
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
                for (int i = 0; i < 4; i++)
                {
                    sf::Vector2f q = gh[i]->sprite.getPosition();
                    float ux = q.x - pos.x, uy = q.y - pos.y;
                    //if (ux * ux + uy * uy < 22.f * 22.f)        // ghost is touching pac-man
                    //{
                    //    Player1.lives--;                         // lose one life
                    //    Player1.sprite.setPosition({ 54, 846 }); // pac-man goes back to start
                    //    direction = ' ';
                    //    rst = true;                              // ghosts go back to the house
                    //    break;                                   // only one life per hit
                    //}
                    // Edited by abdullahameer for orbit speed
                    if (ux * ux + uy * uy < 22.f * 22.f)        // ghost is touching pac-man
                    {
                        if (orbKillActive())
                        {
                            // kill-ghost orb: ghost khatam, score milay, ghost wapas house mein
                            Player1.score += 200 * mult;
                            gr[i] = nr[i] = hr[i];
                            gc[i] = nc[i] = hc[i];
                            vr[i] = 0;
                            vc[i] = 0;
                            gh[i]->sprite.setPosition({ (hc[i] + 0.5f) * CELL, (hr[i] + 0.5f) * CELL });
                            rel[i] = gt + (frenzyOn ? 1.5f : 3.f);   // frenzy: ghosts return twice as fast
                        }
                        else if (!orbInvincible())
                        {
                            // koi orb active nahi: pehle wala lives wala code (bilkul wese hi)
                            Player1.lives--;                         // lose one life
                            Player1.sprite.setPosition({ 54, 846 }); // pac-man goes back to start
                            direction = ' ';
                            orbsReset(false);                        // orb ke timers band (sirf yeh ek line nayi hai)
                            rst = true;                              // ghosts go back to the house
                            break;                                   // only one life per hit
                        }
                        // agar invincible chal raha hai to kuch nahi hota
                    }
                }

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
                // ===== END OF GHOST MOVEMENT =====

                // 3. DRAW
            }

            // working on teleportation

            if (Player1.sprite.getPosition().y >= 398 && Player1.sprite.getPosition().y <= 453)
            {
                if (Player1.sprite.getPosition().x <= 41)
                    Player1.sprite.setPosition({ 860, 414 });
                else if (Player1.sprite.getPosition().x >= 882)
                    Player1.sprite.setPosition({ 43, 414 });
            }


            // take the current score and store it in score value
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

            //take current lives and put then in lives value
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
            // Done by MUHAMMAD KHIZAR REHMAN: draw the grape when it is on the map
            if (gv)
                window.draw(grp);

            // abdullahameer code here

            orbsDraw(window, run_time);

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

        /* window.draw(Blinky.sprite);
         window.draw(blue.sprite);
         window.draw(Yellow.sprite);
         window.draw(Pinky.sprite);*/

         // edit by abd
        for (int i = 0; i < ghostCount; i++)
            if (gh[i]->alive) window.draw(gh[i]->sprite);

        if (levelBanner > 0)                                    // LEVEL n screen
        {
            sf::Text lvlText(font, "LEVEL " + std::to_string(level), 120);
            lvlText.setFillColor(sf::Color::Yellow);
            lvlText.setPosition({ 250.f, 380.f });
            window.draw(lvlText);
        }


        if (Player1.lives <= 0)
        {
            // save the score if not already done (leaderboard code by hmza):
            if (!score_saved)
            {
                addScore(Player1.score, int(run_time));
                score_saved = true;
            }

            window.draw(game_over_screen);
            window.draw(game_over_text);
            window.draw(game_over_text_additional);
            drawLeaderboard(window, font);              // shows the top 5 list (leaderboard code by hmza)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                Player1.lives = 1;
                Player1.score = -10;
                place_dots();
                Player1.sprite.setPosition({ 54, 846 });
                clock.restart();
                direction = ' ';
                run_time = 0;                           // new run starts from zero (leaderboard code by hmza)
                score_saved = false;
                frenzyOn = false;      // frenzy resets on restart (hmza)
                frenzyWait = 0;
                // allow the next game over to save again
                 // abdullahameer code
                orbsReset(true);
                orbsReset(true);
                gv = false;   // Done by MUHAMMAD KHIZAR REHMAN: grape resets on restart
                gtm = 0;

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