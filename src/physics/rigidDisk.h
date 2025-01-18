#ifndef RIGID_DISK_H
#define RIGID_DISK_H

#include <SFML/Graphics.hpp>

struct RigidDisk
{
	float radius{ 20 };
	sf::Vector2f pos{ 0.f, 0.f };
	sf::Vector2f oldPos{ 0.f, 0.f };
	
	sf::Color color;
};

#endif