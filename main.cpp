#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

// this is a test
int main()
{
    // preparing the window
    sf::RenderWindow window(sf::VideoMode({ 900, 900 }), "Pac Man Game");
    window.setFramerateLimit(60);

    // making background

    sf::Texture BackGround("C:\\Users\\rayan\\Documents\\Coding projects\\Pac_Man_Project\\maze.bmp");
    sf::Sprite BackGround_S(BackGround);
    BackGround_S.setScale(
        {
            float(window.getSize().x) / BackGround.getSize().x,
            float(window.getSize().y) / BackGround.getSize().y
        });



    // run the program as long as the window is open
    while (window.isOpen())
    {
        window.clear();
        window.draw(BackGround_S);
        window.display();
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}