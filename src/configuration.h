#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include "common/utility.h"
#include <string_view>

namespace Config
{
	constexpr sf::Vector2u						windowSize		{1920, 1200};
	constexpr sf::Vector2f						windowSizef		{ static_cast<sf::Vector2f>(windowSize) };
	constexpr int								maxFrameRate	{ 60 };	

	constexpr std::pair<sf::Color, sf::Color>	honey			{ { 255, 192, 0 }, {139, 64, 0} };
	constexpr std::pair<sf::Color, sf::Color>	orange			{ {255, 172, 28}, {204, 85, 0 } };


	constexpr sf::Vector2f						gravity			{ 0.f, 1000.f };
	constexpr float								diskRadius		{ 15 };
	constexpr int								spawnRate		{ 2 };
	constexpr bool								autoSpawnStart  { false };
	constexpr std::pair<sf::Color, sf::Color>	diskColor		{ honey }; //available : honey, orange
	constexpr std::string_view					imageFilename	{ "res/arbre.jpg" };


	constexpr bool								showFps			{ false };
	constexpr bool								canScreenMove	{ false };
}

#endif