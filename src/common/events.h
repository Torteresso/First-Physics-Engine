#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Graphics.hpp>
#include "physics/solver.h"

namespace Event
{
	void processEvents(sf::RenderWindow& window, sf::View& view, Solver& solver);

	void moveView(sf::RenderWindow& window, sf::View& view);
}

#endif