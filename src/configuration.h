#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include "common/utility.h"

namespace Config
{
	constexpr sf::Vector2u						windowSize		{1920, 1200};
	constexpr sf::Vector2f						windowSizef		{ static_cast<sf::Vector2f>(windowSize) };
	constexpr int								maxFrameRate	{ 60 };	

	constexpr std::pair<sf::Color, sf::Color> honey		{ { 255, 192, 0 }, {139, 64, 0} };
	constexpr std::pair<sf::Color, sf::Color> orange	{ {255, 172, 28}, {204, 85, 0 } };


	constexpr sf::Vector2f						gravity			{ 0.f, 1000.f };
	constexpr float								diskRadius		{ 10 };
	constexpr int								spawnRate		{ 3 };
	constexpr std::pair<sf::Color, sf::Color>	diskColor		{ honey }; //available : honey, orange



	constexpr bool								showFps			{ false };

}

#endif