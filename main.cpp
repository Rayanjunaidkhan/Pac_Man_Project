#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


// this is a test
// this is hamza
//today is 10/16/26 and we r making pac man.
int main()
{
    sf::Window window(sf::VideoMode({ 800, 600 }), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}