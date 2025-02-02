#include "physics/solver.h"
#include "common/imageToGrid.h"
#include <iostream>
#include <cmath>

Solver::Solver()
{
	std::stringstream os{};
	os << "config" << Config::diskRadius << "_" << Config::spawnRate << ".txt";

	std::string filename{};
	os >> filename;

	std::ifstream IFile{ filename };

	if (!IFile.good()) return;

	std::string cellIdString{};
	while (std::getline(IFile, cellIdString))
	{
		os.str("");
		os.clear();

		os << cellIdString;
		int cellId{};
		os >> cellId;

		m_finalPos.push_back(cellId);
	}

	m_colors = ImageToGrid::transform(Config::imageFilename, m_grid);
}


void Solver::update(const float dt)
{
	const int nbSubStep{ 8 };
	const float subDt{ dt / static_cast<float>(nbSubStep) };
	for (int i{}; i < nbSubStep; i++)
	{
		updateGrid();
		solveCollision();
		updatePos(subDt);
		m_grid.clear();
	}
}

void Solver::updateGrid()
{
	for (int i {}; i < m_objects.size(); i++)
	{
		m_grid.addObject(m_objects[i], i);
	}
}

void Solver::solveCollision()
{
	const int nbCol{ m_grid.getNbCol()};
	for (int k{}; k < m_grid.size(); k++)
	{
		const auto& currentCell{ m_grid[k] };
		for (int j{}; j < currentCell.objCount; j++)
		{
			const int i{ currentCell.objects[j] };
			if (m_grid.idexIsValid(k + nbCol)) solveObjCellCollision(i, m_grid[k + nbCol]);
			if (m_grid.idexIsValid(k + nbCol - 1)) solveObjCellCollision(i, m_grid[k + nbCol - 1]);
			if (m_grid.idexIsValid(k + nbCol + 1)) solveObjCellCollision(i, m_grid[k + nbCol + 1]);
			if (m_grid.idexIsValid(k)) solveObjCellCollision(i, m_grid[k]);
			if (m_grid.idexIsValid(k - 1)) solveObjCellCollision(i, m_grid[k - 1]);
			if (m_grid.idexIsValid(k + 1)) solveObjCellCollision(i, m_grid[k + 1]);
			if (m_grid.idexIsValid(k - nbCol)) solveObjCellCollision(i, m_grid[k - nbCol]);
			if (m_grid.idexIsValid(k - nbCol - 1)) solveObjCellCollision(i, m_grid[k - nbCol - 1]);
			if (m_grid.idexIsValid(k - nbCol + 1)) solveObjCellCollision(i, m_grid[k - nbCol + 1]);
		}
	}
}

void Solver::solveObjCollision(const int id1, const int id2)
{
	auto& obj1{ m_objects[id1] };
	auto& obj2{ m_objects[id2] };

	sf::Vector2f n{ obj2.pos - obj1.pos };

	const float delta = n.length() - obj1.radius - obj2.radius;

	if (delta > 0) return;

	//if (n.length() < 1e-6) n = sf::Vector2f{ 1.f, 1.f };
	if (n.length() < 1e-6) return;

	obj1.pos += 0.5f * delta * n.normalized();
	obj2.pos -= 0.5f * delta * n.normalized();
}

void Solver::solveObjCellCollision(const int id, const Cell& cell)
{
	for (int i{}; i < cell.objCount; i++)
	{
		const int idBis{ cell.objects[i]};
		if (idBis != id) solveObjCollision(id, idBis);
	}
}

void Solver::updatePos(const float dt)
{
	for (auto& obj : m_objects)
	{
		sf::Vector2f newOldPos{ obj.pos };

		const float VELOCITY_DAMPING = 40.0f;

		obj.pos += obj.pos - obj.oldPos + (Config::gravity - (obj.pos - obj.oldPos) * VELOCITY_DAMPING) * dt * dt;
		obj.oldPos = newOldPos;

		applyConstraints(obj);
	}
}

void Solver::applyConstraints(RigidDisk& obj)
{
	const float safety{ 2.f };

	if (obj.pos.y + obj.radius + safety >= (m_grid.getNbRow() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		obj.pos.y = (m_grid.getNbRow() - 2 * m_grid.getPadding()) * m_grid.getCellSize() - obj.radius - safety;
	}
	else if (obj.pos.y - obj.radius - safety <= 0)
	{
		obj.pos.y = obj.radius + safety;
	}

	if (obj.pos.x + obj.radius + safety >= (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		obj.pos.x = (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize() - obj.radius - safety;
	}

	else if (obj.pos.x - obj.radius - safety <= 0)
	{
		obj.pos.x = obj.radius + safety;
	}
}


void Solver::addObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, const sf::Color& color)
{
	const int objId{ static_cast<int>(m_objects.size()) };

	if (m_finalPos.size() > 0 && objId < m_finalPos.size())
	{
		m_objects.push_back({ radius, pos, oldPos, m_colors[m_finalPos[objId]] });
	}
	else
	{
		m_objects.push_back({ radius, pos, oldPos, color });
	}

	if (objId % 1000 == 0) std::cout << "Number of objects : " << m_objects.size() << "\n";
}

void Solver::record() const
{
	std::stringstream os{};
	os << "config" << Config::diskRadius << "_" << Config::spawnRate << ".txt";

	std::string filename{};
	os >> filename;

	std::ifstream IFile{ filename };

	if (IFile.good()) return;

	std::ofstream OFile{ filename };

	for (int i{}; i < m_objects.size(); i++)
	{
		OFile << m_grid.posToIndex(m_objects[i].pos) << "\n";
	}
}
