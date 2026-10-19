#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
struct Player
{
    sf::Sprite sprite;
    int lives = 1;
    int score = 0;
    int current_state = 0; // 0 for normal, 1 for eating ghost mode, 2 for faster speed

    Player(const sf::Texture& tex) : sprite(tex) {}
};

struct Ghost
{
    sf::Sprite sprite;
    int state = 0; // 0  is normal, 1 is scared
    Ghost(const sf::Texture& tex) : sprite(tex) {}
};


void place_dots();

// making constants coz these are fixed mapping values of game.
const int CELL = 25;   // size of one square, in pixels
const int COLS = 36;   // how many squares across
const int ROWS = 36;   // how many squares down
int maze[COLS * ROWS] = {
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // row 0 (top)
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // row 1
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 2
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 3
   1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,   // row 4
   1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,   // row 5
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 6
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 7
   1,1,0,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,1,   // row 8
   1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,   // row 9
   1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,   // row 10
   1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,   // row 11
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 12
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 13
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 14
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // row 16
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // row 17
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 18
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 19
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 20
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 21
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 22
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 23
   1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,   // row 24
   1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,1,   // row 25
   1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,   // row 26
   1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,   // row 27
   1,1,1,1,0,0,0,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,1,   // row 28
   1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,   // row 29
   1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,   // row 30
   1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,   // row 31
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 32
   1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,   // row 33
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // row 34
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1  // row 35 (bottom, no comma at the very end)
};
// ===== ADDING THESE TWO FUNCTIONS HERE COZ THEY ARE FOR MOVEMENT+DEFINED BOUNDARIES =====
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

bool canMoveTo(float x, float y, float half)
{
    if (isWall(x - half, y - half)) return false;
    if (isWall(x + half, y - half)) return false;
    if (isWall(x - half, y + half)) return false;
    if (isWall(x + half, y + half)) return false;
    return true;
}
int main()
{
    // window
    sf::RenderWindow window(sf::VideoMode({ 1200, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);

    // maze background
    sf::Texture BackGround("maze_revised.bmp");
    sf::Sprite BackGround_S(BackGround);
    //BackGround_S.setScale(
    //    {
    //        float(window.getSize().x) / BackGround.getSize().x,
    //        float(window.getSize().y) / BackGround.getSize().y
    //    });

    // player
    sf::Texture Player1_texA("Player1_TextureA.png");
    sf::Texture Player1_texB("Player1_TextureB.png");
    Player Player1(Player1_texA);
    Player1.sprite.setScale(
        {
            50 / float(Player1_texA.getSize().x),
            50 / float(Player1_texA.getSize().y)
        });
    float player1_width = Player1.sprite.getLocalBounds().size.x;
    float player1_height = Player1.sprite.getLocalBounds().size.y;
    Player1.sprite.setOrigin({ player1_width / 2, player1_height / 2 });
    Player1.sprite.setPosition({ 70, 830 });

    // this code is for making the ghost
        // this code is for making red ghost blinky
    sf::Texture Blinky_tex("red_ghost.png");
    Ghost Blinky(Blinky_tex);
    Blinky.sprite.setScale({
        50 / float(Blinky_tex.getSize().x),
        50 / float(Blinky_tex.getSize().y)
        });
    Blinky.sprite.setPosition({ 400,400 });

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
    float speed = 100;       // pixels per second
    float half = 15;         // half of Pac-Man's size (40 / 2) //another change here half=15 from now to make pac-man movement checking defined.
    char direction = ' ';
    bool has_the_run_started = false, current_texture = false;
    bool is_game_paused = false;
    // edges of the open area (from your array)
   //I, Hmzaa is commenting them coz they are here just to define whole maze as a square box and pac man can't go outside but i a using function calling istead of it.
   /* float leftLimit = 50;
    float rightLimit = 850;
    float topLimit = 50;
    float bottomLimit = 850;*/
    //Dots

    //// har 2x2 block mein ek dot
    place_dots();


    // this makes sure that holding esc dosent make pause screen flicker
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        // 1. INPUT
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
        // 2. UPDATE
        if (Player1.lives > 0)
        {
            if (!is_game_paused)
            {
                float dt = clock.restart().asSeconds();

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
                    newY -= speed * dt;
                    has_the_run_started = true;
                }
                else if (direction == 'L')
                {
                    Player1.sprite.setRotation(sf::degrees(180));
                    newX -= speed * dt;
                    has_the_run_started = true;
                }
                else if (direction == 'D')
                {
                    Player1.sprite.setRotation(sf::degrees(90));
                    newY += speed * dt;
                    has_the_run_started = true;
                }
                else if (direction == 'R')
                {
                    Player1.sprite.setRotation(sf::degrees(360));
                    newX += speed * dt;
                    has_the_run_started = true;
                }


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
                for (int r = 0; r < ROWS - 1; r += 2)
                    for (int c = 0; c < COLS - 1; c += 2)
                        if (maze[r * COLS + c] == 2)
                        {
                            float dx = pos.x - (c + 1) * (float)CELL;
                            float dy = pos.y - (r + 1) * (float)CELL;
                            if (dx * dx + dy * dy < 15.f * 15.f)
                            {
                                maze[r * COLS + c] = 0;
                                Player1.score += 10;
                            }
                        }

                // 3. DRAW
            }

            // working on teleportation

            if (Player1.sprite.getPosition().y >= 398 && Player1.sprite.getPosition().y <= 453)
            {
                if (Player1.sprite.getPosition().x <= 41)
                    Player1.sprite.setPosition({ 860, 421 });
                else if (Player1.sprite.getPosition().x >= 882)
                    Player1.sprite.setPosition({ 43, 421 });
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
            window.draw(BackGround_S);
            window.draw(SideScreen);
            window.draw(Game_Title);
            window.draw(Score_counter);
            window.draw(Score_value);
            window.draw(Lives_counter);
            window.draw(Lives_value);

            // TEST: red squares over wall cells (delete when walls are finished)
            sf::RectangleShape box({ float(CELL), float(CELL) });
            box.setFillColor(sf::Color(255, 0, 0, 120));
            for (int row = 0; row < ROWS; row++)
            {
                for (int col = 0; col < COLS; col++)
                {
                    if (maze[row * COLS + col] == 1)
                    {
                        box.setPosition({ float(col * CELL), float(row * CELL) });
                        window.draw(box);
                    }
                }
            }
            //draw dots
            sf::CircleShape dot(3.f);
            dot.setFillColor(sf::Color::Yellow);

            for (int r = 0; r < ROWS - 1; r += 2)
                for (int c = 0; c < COLS - 1; c += 2)
                    if (maze[r * COLS + c] == 2)
                    {
                        dot.setPosition({ (c + 1) * (float)CELL - 3.f, (r + 1) * (float)CELL - 3.f });
                        window.draw(dot);
                    }

        }
        window.draw(Player1.sprite);   // last, so he is on top
        if (is_game_paused)
        {
            window.draw(overlay);
            window.draw(paused_text);
            window.draw(paused_text_additional);
        }

        window.draw(Blinky.sprite);

        if (Player1.lives <= 0)
        {
            window.draw(game_over_screen);
            window.draw(game_over_text);
            window.draw(game_over_text_additional);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                Player1.lives = 1;
                Player1.score = 0;
                place_dots();
                Player1.sprite.setPosition({ 70, 830 });
                clock.restart();
                direction = ' ';
            }
        }
        window.display();

    }
}

void place_dots()
{
    for (int i = 0; i < ROWS - 1; i += 2)
    {
        for (int j = 0; j < COLS - 1; j += 2)
        {
            int a = i * COLS + j;          // top-left
            int b = i * COLS + j + 1;      // top-right
            int c = (i + 1) * COLS + j;    // bottom-left
            int d = (i + 1) * COLS + j + 1; // bottom-right

            // agar chaaron khali hain to dot lagao
            if (maze[a] == 0 && maze[b] == 0 && maze[c] == 0 && maze[d] == 0)
            {
                maze[a] = 2;
            }
        }
    }
}