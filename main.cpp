#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
//ss
// this is a test
// this is hamza
//today is 10/16/26 and we r making pac man.
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
            40 / float(Player1_texA.getSize().x),
            40 / float(Player1_texA.getSize().y)
        });

    // set player 1 origin to its center

    float player1_width = Player1.getLocalBounds().size.x;
    float player1_height = Player1.getLocalBounds().size.y;
    Player1.setOrigin({ player1_width / 2, player1_height / 2 });
    
    // make the player start at the start gate
    Player1.setPosition({ 80, 830 });


    // declararion of random variables
    const int speed = 55;
    char direction = ' ';
    // run the program as long as the window is open
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        //deal with player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            direction = 'U';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            direction = 'L';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            direction = 'D';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            direction = 'R';


        if (direction == 'U')
        {
            Player1.setRotation(sf::degrees(90));
            Player1.move({ 0,-speed * dt });
        }
        else if (direction == 'L')
        {
            Player1.setRotation(sf::degrees(360));
            Player1.move({ -speed * dt,0 });
        }
        else if (direction == 'D')
        {
            Player1.setRotation(sf::degrees(270));
            Player1.move({ 0,speed * dt });
        }
        else if (direction == 'R')
        {
            Player1.setRotation(sf::degrees(180));
            Player1.move({ speed * dt,0 });
        }
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