#include "physics/solver.h"
#include "common/imageToGrid.h"
#include <iostream>
#include <cmath>

Solver::Solver()
{
	m_pool = ObjectPool(2 * m_grid.size());
	addInitialConfig();

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

	m_colors = ImageToGrid::ColorTransform(Config::imageFilename, m_grid);
}

void Solver::addInitialConfig()
{
	const int holeSize{ 2 };
	const int nbLines{ 1 };
	const int spaceBetweenLines{ 3 };
	int alternating{ 0 };
	const int spacing{ 1 };
	for (int l{}; l < nbLines * spaceBetweenLines; l += spaceBetweenLines)
	{
		alternating += spacing / 2;
		for (int col{ m_grid.getPadding() + holeSize }; col < m_grid.getNbCol() - m_grid.getPadding(); col += spacing)
		{
			const int cellId{ (m_grid.getNbRow() - l - 2 * m_grid.getPadding()) * m_grid.getNbCol() + col + alternating % 2 };
			const sf::Vector2f pos{ m_grid.getCellMiddlePos(cellId) };
			addDisk(Config::diskRadius, pos, pos, sf::Color::White, true);
		}
	}
}

void Solver::update(const float dt)
{
	const int nbSubStep{ 8 };
	const float subDt{ dt / static_cast<float>(nbSubStep) };
	for (int i{}; i < nbSubStep; i++)
	{
		updateGrid();
		solveLinks();
		solveCollision();
		updatePos(subDt);
		m_grid.clear();
		clean();
	}
}

void Solver::clean()
{
	for (int i{}; i < m_disks.size(); i++)
	{
		if (!m_disks[i]->isDead) continue;

		for (auto& diskObj : m_diskObjects)
		{
			if (!diskObj.contains(*m_disks[i])) continue;


			diskObj.clean();
			break;
		}

		m_pool.Free(m_disks[i]);

		auto it = m_disks.begin();
        std::advance(it, i);
        m_disks.erase(it);

	}
}

void Solver::solveLinks()
{
	for (auto& diskObj : m_diskObjects)
	{
		diskObj.applyLinks();
	}
}

void Solver::updateGrid()
{
	for (int i {}; i < m_disks.size(); i++)
	{
		m_grid.addDisk(*m_disks[i], i);
	}
}

void Solver::solveCollision()
{
	const int nbCol{ m_grid.getNbCol()};
	for (int k{}; k < m_grid.size(); k++)
	{
		const auto& currentCell{ m_grid[k] };
		for (int j{}; j < currentCell.diskCount; j++)
		{
			const int i{ currentCell.disks[j] };
			if (m_grid.idexIsValid(k + nbCol)) solveDiskCellCollision(i, m_grid[k + nbCol]);
			if (m_grid.idexIsValid(k + nbCol - 1)) solveDiskCellCollision(i, m_grid[k + nbCol - 1]);
			if (m_grid.idexIsValid(k + nbCol + 1)) solveDiskCellCollision(i, m_grid[k + nbCol + 1]);
			if (m_grid.idexIsValid(k)) solveDiskCellCollision(i, m_grid[k]);
			if (m_grid.idexIsValid(k - 1)) solveDiskCellCollision(i, m_grid[k - 1]);
			if (m_grid.idexIsValid(k + 1)) solveDiskCellCollision(i, m_grid[k + 1]);
			if (m_grid.idexIsValid(k - nbCol)) solveDiskCellCollision(i, m_grid[k - nbCol]);
			if (m_grid.idexIsValid(k - nbCol - 1)) solveDiskCellCollision(i, m_grid[k - nbCol - 1]);
			if (m_grid.idexIsValid(k - nbCol + 1)) solveDiskCellCollision(i, m_grid[k - nbCol + 1]);
		}
	}
}

void Solver::solveDiskCollision(const int id1, const int id2)
{
	auto& disk1{ *m_disks[id1] };
	auto& disk2{ *m_disks[id2] };

	if (disk1.isVirtual || disk2.isVirtual) return;

	sf::Vector2f n{ disk2.pos - disk1.pos };

	const float delta = n.length() - disk1.radius - disk2.radius;

	if (delta > 0) return;

	//if (n.length() < 1e-6) n = sf::Vector2f{ 1.f, 1.f };
	if (n.length() < 1e-6) return;

	float disk1Displacement{ 0.5f};

	if (disk1.fixed)
	{
		disk1Displacement = 0.f;
	}
	else if (disk2.fixed)
	{
		disk1Displacement = 1.f;
	}

	const float disk2Displacement{ 1.f - disk1Displacement };

	disk1.pos += disk1Displacement * delta * n.normalized();
	disk2.pos -= disk2Displacement * delta * n.normalized();
}

void Solver::solveDiskCellCollision(const int id, const Cell& cell)
{
	for (int i{}; i < cell.diskCount; i++)
	{
		const int idBis{ cell.disks[i]};
		if (idBis != id) solveDiskCollision(id, idBis);
	}
}

void Solver::updatePos(const float dt)
{
	for (auto& diskPtr : m_disks)
	{
		auto& disk{ *diskPtr };

		if (disk.fixed) continue;

		sf::Vector2f newOldPos{ disk.pos };

		const float VELOCITY_DAMPING = 40.0f;

		disk.pos += disk.pos - disk.oldPos + (Config::gravity - (disk.pos - disk.oldPos) * VELOCITY_DAMPING) * dt * dt;
		disk.oldPos = newOldPos;

		applyConstraints(disk);
	}
}


void Solver::removeObjectsFromDisk(const RigidDisk& disk)
{
	for (int i {}; i < m_diskObjects.size(); i++)
	{
		if (m_diskObjects[i].contains(disk))
		{
			m_diskObjects[i].explode();
			m_diskObjects.erase(m_diskObjects.begin() + i);
			return;
		}
	}
}

void Solver::applyConstraints(RigidDisk& disk)
{
	const float safety{ 2.f };

	if (disk.pos.y - 4 * disk.radius + safety >= (m_grid.getNbRow() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		disk.isDead = true;
	}
	else if (disk.pos.y - disk.radius - safety <= 0)
	{
		disk.pos.y = disk.radius + safety;
	}

	if (disk.pos.x + disk.radius + safety >= (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		disk.pos.x = (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize() - disk.radius - safety;
	}

	else if (disk.pos.x - disk.radius - safety <= 0)
	{
		disk.pos.x = disk.radius + safety;
	}
}


void Solver::addDisk(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, 
					 const sf::Color& color, const bool fixed, const bool isVirtual)
{
	sf::Vector2f newPos{ pos };
	sf::Vector2f newOldPos{ oldPos };

	if (fixed)
	{
		const int cellId{ m_grid.posToIndex(pos) };
		if (m_occupied.count(cellId))
		{
			return;
		}

		m_occupied.insert(cellId);

		newPos = m_grid.getCellMiddlePos(cellId);
		newOldPos = newPos;
	}

	const int diskId{ static_cast<int>(m_disks.size()) };

		
	RigidDisk* disk = m_pool.Alloc();

	disk->radius = radius;
	disk->pos = newPos;
	disk->oldPos = newOldPos;
	disk->color = color;
	disk->fixed = fixed;
	disk->isVirtual = isVirtual;

	if (m_finalPos.size() > 0 && diskId < m_finalPos.size() && !fixed)
	{
			disk->color = m_colors[m_finalPos[diskId]];
	}

	m_disks.emplace_back(disk);

	if (diskId % 1000 == 0) std::cout << "Number of disks : " << m_disks.size() << "\n";
}

void Solver::addDiskForObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, const sf::Color& color)
{
	const int diskId{ static_cast<int>(m_disks.size()) };

	addDisk(radius, pos, oldPos, color, true, true);

	if (m_disks.size() == diskId) return;  //No new DISK has been added

	m_objDiskComponents.push_back(m_disks.back());
}

void Solver::addObject()
{
	for (auto& disk : m_objDiskComponents)
	{
		const int cellId{ m_grid.posToIndex(disk->pos)};
		m_occupied.erase(cellId);

		disk->isVirtual = false;
		disk->fixed = false;
	}

	m_diskObjects.push_back(DiskObject(m_objDiskComponents));

	m_objDiskComponents.clear();
}

void Solver::clear()
{
	m_grid.clear();
	m_disks.clear();
	m_pool.clear();
	m_objDiskComponents.clear();
	m_diskObjects.clear();
	m_occupied.clear();
	addInitialConfig();
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

	for (int i{}; i < m_disks.size(); i++)
	{
		OFile << m_grid.posToIndex(m_disks[i]->pos) << "\n";
	}

	std::cout << "CONFIGURATION SAVED SUCCESSFULLY\n";
}

void Solver::updateDebug()
{

	if (m_disks.size() < 1) return;

	if (m_diskViewed >= static_cast<int>(m_disks.size()))
	{
		m_diskViewed = 0;
	}
	else if (m_diskViewed < 0)
	{
		m_diskViewed = m_disks.size() - 1;
	}
}

void Solver::viewNext()
{
	++m_diskViewed;
	updateDebug();
}

void Solver::viewPrevious()
{
	--m_diskViewed;
	updateDebug();
}
