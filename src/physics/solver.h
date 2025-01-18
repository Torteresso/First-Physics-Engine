#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "physics/rigidDisk.h"
#include "configuration.h"

class Solver
{
public:
	Solver()
	{
	}

	void update(const float dt);

	void updatePos(const float dt);
	void applyConstraints();
	void solveCollision();

	const std::vector<RigidDisk>& getObjects() const { return m_objects; }

	void addObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& olfPos, const sf::Color& color);
private:
	std::vector<RigidDisk> m_objects;

};

#endif