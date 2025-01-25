#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "physics/rigidDisk.h"
#include "configuration.h"
#include "physics/grid.h"
#include <SFML/Graphics.hpp>

class Solver
{
public:
	Solver()
	{
	}

	void update(const float dt);

	void updatePos(const float dt);
	void applyConstraints(RigidDisk& obj);
	
	void solveCollision();
	void solveCollisionNaive();
	void solveObjCellCollision(const int id, const Cell& cell2);
	void solveObjCollision(const int id1, const int id2);

	void updateGrid();

	const std::vector<RigidDisk>& getObjects() const { return m_objects; }

	void addObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& olfPos, const sf::Color& color);
private:
	std::vector<RigidDisk> m_objects;

	Grid m_grid{};
};

#endif