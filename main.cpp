#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
//ss
// this is a test
int main()
{
    // make clock to see how much time elapsed
    sf::Clock clock;
    // preparing the window
    sf::RenderWindow window(sf::VideoMode({ 900, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);

    // making background

    sf::Texture BackGround("maze.bmp");
    sf::Sprite BackGround_S(BackGround);
    BackGround_S.setScale(
        {
            float(window.getSize().x) / BackGround.getSize().x,
            float(window.getSize().y) / BackGround.getSize().y
        });

    // now we will make the player 1 character
    sf::Texture Player1_texA("Player1_TextureA.bmp");
    sf::Sprite Player1(Player1_texA);
    Player1.setScale(
        {
            50 / float(Player1_texA.getSize().x),
            50 / float(Player1_texA.getSize().y)
        });

    // run the program as long as the window is open
    while (window.isOpen())
    {
        window.clear();
        window.draw(BackGround_S);
        window.draw(Player1);
        window.display();
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}