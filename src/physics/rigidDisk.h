#ifndef RIGID_DISK_H
#define RIGID_DISK_H

#include <SFML/Graphics.hpp>

class RigidDisk
{
public:
	RigidDisk() = delete;

	RigidDisk(float radius, sf::Vector2f pos, sf::Color c) : m_pos{ pos }, m_oldPos{ pos }, m_color{ c }, m_radius{radius}
	{
	}

	const sf::Vector2f& getPos() const{ return m_pos; }
	const sf::Color& getColor() const { return m_color; }
	const float getRadius() const { return m_radius; }

private:

	sf::Vector2f m_pos;
	sf::Vector2f m_oldPos;
	
	sf::Color m_color;

	float m_radius;
};

#endif