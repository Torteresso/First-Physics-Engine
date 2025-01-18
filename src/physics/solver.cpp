#include "physics/solver.h"

void Solver::addObject(float radius, const sf::Vector2f& pos, const sf::Color& color)
{
	m_objects.push_back(RigidDisk(radius, pos, color));
}
