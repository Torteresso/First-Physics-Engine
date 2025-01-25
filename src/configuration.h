#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>

namespace Config
{
	sf::Vector2u constexpr windowSize {1920, 1200};
	sf::Vector2f constexpr windowSizef{ static_cast<sf::Vector2f>(windowSize) };
	int constexpr maxFrameRate{ 60 };

	constexpr sf::Vector2f gravity{ 0.f, 1000.f };
	constexpr float diskRadius{ 5 };

	constexpr bool showFps{ true };
}

#endif