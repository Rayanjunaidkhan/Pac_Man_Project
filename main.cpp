#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
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
   1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,   // row 12
   1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,   // row 13
   1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,   // row 14
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // row 16
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // row 17
   1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,   // row 18
   1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,   // row 19
   1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,   // row 20
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
    sf::RenderWindow window(sf::VideoMode({ 900, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);

    // maze background
    sf::Texture BackGround("maze.bmp");
    sf::Sprite BackGround_S(BackGround);
    BackGround_S.setScale(
        {
            float(window.getSize().x) / BackGround.getSize().x,
            float(window.getSize().y) / BackGround.getSize().y
        });

    // player
    sf::Texture Player1_texA("Player1_TextureA.png");
    sf::Texture Player1_texB("Player1_TextureB.bmp");
    sf::Sprite Player1(Player1_texA);
    Player1.setScale(
        {
            40 / float(Player1_texA.getSize().x),
            40 / float(Player1_texA.getSize().y)
        });
    float player1_width = Player1.getLocalBounds().size.x;
    float player1_height = Player1.getLocalBounds().size.y;
    Player1.setOrigin({ player1_width / 2, player1_height / 2 });
    Player1.setPosition({ 80, 830 });

    // variables (each declared ONCE)
    sf::Clock clock, timer_for_texture_change;
    float speed = 100;       // pixels per second
    float half = 15;         // half of Pac-Man's size (40 / 2) //another change here half=15 from now to make pac-man movement checking defined.
    char direction = ' ';
    bool has_the_run_started = false, current_texture = false;
    // edges of the open area (from your array)
   //I, Hmzaa is commenting them coz they are here just to define whole maze as a square box and pac man can't go outside but i a using function calling istead of it.
   /* float leftLimit = 50;
    float rightLimit = 850;
    float topLimit = 50;
    float bottomLimit = 850;*/
    //Dots
    int score = 0;

    //// har 2x2 block mein ek dot
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

    while (window.isOpen())
    {
        // 1. INPUT
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 2. UPDATE
        float dt = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            direction = 'U';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            direction = 'L';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            direction = 'D';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            direction = 'R';

        // where he is now, and where he WANTS to go
        sf::Vector2f pos = Player1.getPosition();
        float newX = pos.x;
        float newY = pos.y;

        if (direction == 'U')
        {
            Player1.setRotation(sf::degrees(270));
            newY -= speed * dt;
            has_the_run_started = true;
        }
        else if (direction == 'L')
        {
            Player1.setRotation(sf::degrees(180));
            newX -= speed * dt;
            has_the_run_started = true;
        }
        else if (direction == 'D')
        {
            Player1.setRotation(sf::degrees(90));
            newY += speed * dt;
            has_the_run_started = true;
        }
        else if (direction == 'R')
        {
            Player1.setRotation(sf::degrees(360));
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
                    Player1.setTexture(Player1_texA);
                else
                    Player1.setTexture(Player1_texB);
                timer_for_texture_change.restart();
            }

        }
        // only move if the wanted spot is inside the limits
        if (canMoveTo(newX, pos.y, half))
            pos.x = newX;
        if (canMoveTo(pos.x, newY, half))
            pos.y = newY;

        Player1.setPosition(pos);
        for (int r = 0; r < ROWS - 1; r += 2)
            for (int c = 0; c < COLS - 1; c += 2)
                if (maze[r * COLS + c] == 2)
                {
                    float dx = pos.x - (c + 1) * (float)CELL;
                    float dy = pos.y - (r + 1) * (float)CELL;
                    if (dx * dx + dy * dy < 15.f * 15.f)
                    {
                        maze[r * COLS + c] = 0;
                        score++;
                    }
                }

                // 3. DRAW
        window.clear();
        window.draw(BackGround_S);

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
        

        window.draw(Player1);   // last, so he is on top
        window.display();

    }
}