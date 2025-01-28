#ifndef IMAGE_TO_GRID_H
#define IMAGE_TO_GRID_H

#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>

#include "physics/grid.h"

namespace ImageToGrid
{
	std::vector<sf::Color> transform(std::string_view filename, const Grid& grid)
	{
		
		std::vector<sf::Color> colorGrid;
		colorGrid.reserve(grid.sizeWithoutPadding());

		

		return colorGrid;
	}
}

#endif