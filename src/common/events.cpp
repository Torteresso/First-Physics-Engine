#include "common/events.h"
#include "common/random.h"
#include "common/imageToGrid.h"

#include <iostream>
#include <cmath>

bool g_mouseLeftDown{ false };

void Event::processEvents(sf::RenderWindow& window, sf::View& view, Solver& solver, bool& spawnDisks)
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
				
			}
		}
		else if (const auto& keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
			{
				spawnDisks = !spawnDisks;
			}
			if (keyReleased->scancode == sf::Keyboard::Scancode::S)
			{
				solver.record();
			}
			if (keyReleased->scancode == sf::Keyboard::Scancode::C)
			{
				solver.clear();
			}
			if (keyReleased->scancode == sf::Keyboard::Scancode::L)
			{
				const float ratio{ 1.f };
				ImageToGrid::ObjectTransform("res/A.png", solver,
					{ static_cast<float>(Random::get(1, Config::windowSize.x - 201 * ratio))
					 ,static_cast<float>(Random::get(1, Config::windowSize.y - 201 * ratio)) }, ratio);
			}
			if (keyReleased->scancode == sf::Keyboard::Scancode::Right)
			{
				if (Config::debug) solver.viewNext();
			}
			if (keyReleased->scancode == sf::Keyboard::Scancode::Left)
			{
				if (Config::debug) solver.viewPrevious();
			}


		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				if (!g_mouseLeftDown) 
					solver.addDiskForObject(Config::diskRadius,
					window.mapPixelToCoords(mouseButtonPressed->position),
					window.mapPixelToCoords(mouseButtonPressed->position),
					sf::Color::White);
				g_mouseLeftDown = true;

			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = false;
			}
			if (mouseButtonReleased->button == sf::Mouse::Button::Right)
			{
				solver.addObject();
			}

		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			if (g_mouseLeftDown)
				solver.addDiskForObject(Config::diskRadius,
					window.mapPixelToCoords(mouseMoved->position),
					window.mapPixelToCoords(mouseMoved->position),
					sf::Color::White);

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