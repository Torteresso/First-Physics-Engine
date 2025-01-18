#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "physics/rigidDisk.h"

class Solver
{
public:
	Solver()
	{
	}

	//void update

	const std::vector<RigidDisk>& getObjects() const { return m_objects; }

	void addObject(float radius, const sf::Vector2f& pos, const sf::Color& color);
private:
	std::vector<RigidDisk> m_objects;

};

#endif