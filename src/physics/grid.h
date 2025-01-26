#ifndef GRID_H
#define GRID_H

#include <vector>
#include "array"
#include <SFML/Graphics.hpp>
#include "physics/rigidDisk.h"
#include "configuration.h"
#include <iostream>

struct Cell
{
	static constexpr int capacity{ 4 };
	static constexpr int maxIndex{ 3 };

	int id{};

	int objCount{ 0 };
	std::array<int, capacity> objects {};

	Cell() = default;

	void addObject(const int index)
	{
		objects[objCount] = index;
		if (objCount > 3) std::cout << id << ", " << objCount << "wow\n";
		objCount += objCount < maxIndex;
	}

	void clear()
	{
		objCount = 0;
	}
};

class Grid
{
	using DiskGrid = std::vector<Cell>;

	static constexpr float m_cellSize{ 2 * Config::diskRadius };
	static constexpr int m_padding{ 1 };
	static constexpr int m_nbRow{static_cast<int>(Config::windowSize.y / (2*Config::diskRadius)) + 2 * m_padding};
	static constexpr int m_nbCol{static_cast<int>(Config::windowSize.x / (2*Config::diskRadius)) + 2 * m_padding};

public:
	Grid()
	{
		m_disks.resize(m_nbCol * m_nbRow);
		std::cout << (m_nbCol - 1) * m_nbRow << "\n";

		for (int i{}; i < m_nbCol * m_nbRow; i++)
		{
			m_disks[i].id = i;
		}
	}

	void addObject(RigidDisk& obj, const int id)
	{
		m_disks[posToIndex(obj.pos)].addObject(id);
	}

	void clear()
	{
		for (auto& elt : m_disks) elt.clear();
	}
	
	const int size() const
	{
		return m_disks.size();
	}

	Cell& operator[] (int index)
	{
		return m_disks[index];
	}

	const int getNbCol() const{ return m_nbCol; }
	const int getNbRow() const{ return m_nbRow; }
	const int getCellSize() const { return m_cellSize; }
	const int getPadding() const { return m_padding; }

	static const int coordToIndex(const int x, const int y)
	{
		return x + y * m_nbCol;
	}

	const bool idexIsValid(const int i)
	{
		return (i >= 0 && i < m_disks.size());
	}

private:

	static const int posToIndex(const sf::Vector2f& pos)
	{
		return (static_cast<int>(pos.y / m_cellSize) + m_padding) * m_nbCol + static_cast<int>(pos.x / m_cellSize) + m_padding;
	}

	DiskGrid m_disks;
};

#endif
