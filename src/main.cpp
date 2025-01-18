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

    const float dt = 1.f / static_cast<float>(Config::maxFrameRate);

    while (window.isOpen())
    {
        Event::processEvents(window, mainView, solver);
        Event::moveView(window, mainView);

        solver.update(dt);

        window.clear();
        
        window.setView(mainView);
        renderer.render(window);

        window.display();
    }
}
