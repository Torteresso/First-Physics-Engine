#include <SFML/Graphics.hpp>

#include <iostream>

#include "common/events.h"
#include "configuration.h"
#include "renderer/renderer.h"
#include "physics/solver.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

    sf::View mainView = window.getDefaultView();

    Solver solver{};
    Renderer renderer{ solver };

    sf::Texture texture;
    
       texture.setSmooth(true);

    sf::VertexArray square(sf::PrimitiveType::Triangles, 6);
   
    square[0].position = sf::Vector2f(100.0f, 100.0f);
    square[1].position = sf::Vector2f(350.0f, 100.0f);
    square[2].position = sf::Vector2f(350.0f, 350.0f);
    square[3].position = sf::Vector2f(100.0f, 100.0f);
    square[4].position = sf::Vector2f(100.0f, 350.0f);
    square[5].position = sf::Vector2f(350.0f, 350.0f);

    square[0].color = sf::Color::Green;
    square[1].color = sf::Color::Green;
    square[2].color = sf::Color::Green;
    square[3].color = sf::Color::Green;
    square[4].color = sf::Color::Green;
    square[5].color = sf::Color::Green;

    square[0].texCoords = sf::Vector2f(0.f, 0.f);
    square[1].texCoords = sf::Vector2f(1024.f, 0.f);
    square[2].texCoords = sf::Vector2f(1024.f, 1024.f);
    square[3].texCoords = sf::Vector2f(0.f, 0.f);
    square[4].texCoords = sf::Vector2f(0.f, 1024.f);
    square[5].texCoords = sf::Vector2f(1024.f, 1024.f);

    while (window.isOpen())
    {
        Event::processEvents(window, mainView, solver);
        Event::moveView(window, mainView);

        window.clear();
        
        window.setView(mainView);
        //window.draw(square, &texture);
        renderer.render(window);

        window.display();
    }
}
