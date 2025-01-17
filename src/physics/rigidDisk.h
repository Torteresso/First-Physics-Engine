#ifndef RIGID_DISK_H
#define RIGID_DISK_H

#include <SFML/Graphics.hpp>

class RigidDisk
{
public:
	RigidDisk(float radius, sf::Color c, sf::Vector2f pos) : m_pos{ pos }, m_oldPos{ pos }
	{
		m_body.setFillColor(c);
		m_body.setPosition(pos);
		m_body.setRadius(radius);
	}

private:
	sf::CircleShape m_body;

	sf::Vector2f m_pos;
	sf::Vector2f m_oldPos;
};

#endif