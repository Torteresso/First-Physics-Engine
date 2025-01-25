#include "common/events.h"
#include "common/random.h"

#include <iostream>
#include <cmath>

bool g_mouseLeftDown{ false };

void Event::processEvents(sf::RenderWindow& window, sf::View& view, Solver& solver)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();

			if (keyPressed->scancode == sf::Keyboard::Scancode::R)
			{
				view = window.getDefaultView();
			}

			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				const int nbToadd{ 3 };
				for (int i{}; i < nbToadd; i++)
				{
					solver.addObject(Config::diskRadius,
						{ 100 + 3 * i * Config::diskRadius, 100 + 3 * i * Config::diskRadius },
						{ 97 + 3 * i * Config::diskRadius, 100 + 3 * i * Config::diskRadius },
						{ static_cast<uint8_t>(Random::get(0, 255)),
						  static_cast<uint8_t>(Random::get(0, 255)),
						  static_cast<uint8_t>(Random::get(0, 255)) });
				}

			}
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = true;
			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = false;
				solver.addObject(Config::diskRadius,
								 window.mapPixelToCoords(mouseButtonReleased->position),
								 window.mapPixelToCoords(mouseButtonReleased->position),
								 { static_cast<uint8_t>(Random::get(0, 255)),
								   static_cast<uint8_t>(Random::get(0, 255)),
								   static_cast<uint8_t>(Random::get(0, 255)) });
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{

		}
		else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
		{
			view.zoom(1.0f - mouseWheelScrolled->delta / 20.0f);
		}
	}
}

void Event::moveView(sf::RenderWindow& window, sf::View& view)
{
	sf::Vector2f scroll = window.mapPixelToCoords(sf::Mouse::getPosition()) - view.getCenter();
	const float velocity{ view.getSize().x / 800};
	const float dissipation{ 0.2f };

	const float halfWidth{ view.getSize().x / 2 };
	const float halfHeight{ view.getSize().y / 2};

	const float portionTriger{ 0.9 };

	if (scroll.x * scroll.x > halfWidth * halfWidth * portionTriger * portionTriger
		|| scroll.y * scroll.y > halfHeight * halfHeight * portionTriger * portionTriger)
	{
		const int signX{ (scroll.x > 0) ? 1 : ((scroll.x < 0) ? -1 : -1) };
		const int signY{ (scroll.y > 0) ? 1 : ((scroll.y < 0) ? -1 : -1) };

		if (scroll.x > 0)
		{
			(scroll.x > halfWidth * portionTriger) ? scroll.x -= halfWidth * portionTriger : scroll.x = 0;
		}
		else (scroll.x < -halfWidth *portionTriger) ? scroll.x += halfWidth * portionTriger : scroll.x = 0;
		if (scroll.y > 0) (scroll.y > halfHeight * portionTriger) ? scroll.y -= halfHeight * portionTriger : scroll.y = 0;
		else (scroll.y < -halfHeight * portionTriger) ? scroll.y -= halfHeight * portionTriger : scroll.y = 0;

		view.move({ signX *std::pow(std::abs(scroll.x), dissipation) * velocity , signY * std::pow(std::abs(scroll.y), dissipation) * velocity});
	}
}