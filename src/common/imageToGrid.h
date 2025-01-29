#ifndef IMAGE_TO_GRID_H
#define IMAGE_TO_GRID_H

#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics/grid.h"

namespace ImageToGrid
{
	std::vector<sf::Color> transform(std::string_view filename, const Grid& grid)
	{
		sf::Image image{ filename };
		
		std::vector<sf::Color> colorGrid;
		colorGrid.reserve(grid.size());

		const int gridNbRow{ grid.getNbRow()};
		const int gridNbCol{ grid.getNbCol()};

		const int imageCellSizeX{ static_cast<int>(image.getSize().x / gridNbCol) };
		const int imageCellSizeY{ static_cast<int>(image.getSize().y / gridNbRow) };

		for (int row{}; row < gridNbRow; row++)
		{
			for (int col{}; col < gridNbCol; col++)
			{
				sf::Vector2u imageCoord{static_cast<unsigned>(col * imageCellSizeX), static_cast<unsigned>(row * imageCellSizeY)};

				const sf::Color pixelColor{ image.getPixel(imageCoord) };
				colorGrid.push_back(pixelColor);

				//std::cout << imageCoord.x << ", " << imageCoord.y 
					//<< ", (" << static_cast<int>(pixelColor.r) << ", " << static_cast<int>(pixelColor.g)
					//<< ", " << static_cast<int>(pixelColor.b) << ")\n";
			}
		}

		return colorGrid;
	}
}

#endif