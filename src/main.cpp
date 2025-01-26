#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "common/events.h"
#include "configuration.h"
#include "renderer/renderer.h"
#include "physics/solver.h"
#include "physics/grid.h"
#include "common/utility.h"
#include "common/random.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

    sf::View mainView = window.getDefaultView();

    sf::Clock clock;

    Solver solver{};
    Renderer renderer{ solver };

    const float dt = 1.f / static_cast<float>(Config::maxFrameRate);

    std::vector<float> fpsVector{};
    if (Config::showFps) fpsVector.reserve(1000);

    bool spawnObjects{ true };

    sf::Time colorTime;


    while (window.isOpen())
    {
        Event::processEvents(window, mainView, solver, spawnObjects);
        //Event::moveView(window, mainView);

        sf::Time elapsed = clock.restart();
        colorTime += elapsed;

        if (Config::showFps)
        {
            float fps{ 1.0f / elapsed.asSeconds() };
            if (fps <= Config::maxFrameRate)
                fpsVector.push_back(fps);

            if (fpsVector.size() > 50)
            {
				std::cout << "FPS : " << Utility::average(fpsVector) << "\n";
                fpsVector.clear();
            }

        }

        if (spawnObjects)
        {
            if (solver.getObjects().size() >= solver.getMaxObjects() - Config::spawnRate) spawnObjects = false;

            for (int i{}; i < Config::spawnRate; i++)
            {
                solver.addObject(Config::diskRadius,
                    { 100 + 3 * i * Config::diskRadius, 100 + 3 * i * Config::diskRadius },
                    { 97 + 3 * i * Config::diskRadius, 100 + 3 * i * Config::diskRadius },
                    Utility::getGradient(colorTime.asSeconds(), Config::diskColor.first, Config::diskColor.second));
            }
        }

        solver.update(dt);

        window.clear();
        
        window.setView(mainView);
        renderer.render(window);

        window.display();
    }
}
