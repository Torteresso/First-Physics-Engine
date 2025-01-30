#ifndef RIGID_DISK_H
#define RIGID_DISK_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include "configuration.h"

struct RigidDisk
{
	float radius{ 20 };
	sf::Vector2f pos{ 0.f, 0.f };
	sf::Vector2f oldPos{ 0.f, 0.f };
	
	sf::Color color;

	bool fixed{ false };
	bool isVirtual{ false };
};

class Link
{
public:

	Link(RigidDisk& disk1, RigidDisk& disk2, const float targetDist) : m_disk1{ disk1 }, m_disk2{ disk2 }, m_targetDist{ targetDist }
	{

	}

	void apply()
	{
		sf::Vector2f n{ m_disk2.pos - m_disk1.pos };
		
		float delta{ n.length() - m_targetDist };
	
		if (n.length() < 1e-6) return;

		float disk1Displacement{ 0.5f};

		if (m_disk1.fixed)
		{
			disk1Displacement = 0.f;
		}
		else if (m_disk2.fixed)
		{
			disk1Displacement = 1.f;
		}

		const float disk2Displacement{ 1.f - disk1Displacement };

		m_disk1.pos += disk1Displacement * delta * n.normalized();
		m_disk2.pos -= disk2Displacement * delta * n.normalized();

	}

	RigidDisk& m_disk1;
	RigidDisk& m_disk2;
	float m_targetDist{};

};

class DiskObject
{
public:
	DiskObject(std::vector<RigidDisk*>& components) : m_components {components}
	{
		if (m_components.size() <= 1) return;

		const float dist{ (m_components[0]->pos - m_components[1]->pos).length() };
		m_links.push_back({ *m_components[0], *m_components[1], 2 * Config::diskRadius });
		
		if (m_components.size() == 2) return;

		for (int i{2}; i < m_components.size(); i++)
		{
			auto& disk1{ *m_components[i - 2] };
			auto& disk2{ *m_components[i - 1] };
			auto& disk3{ *m_components[i] };
			const float dist23{ (disk3.pos - disk2.pos).length() };
			const float dist13{ (disk3.pos - disk1.pos).length() };

			m_links.push_back({ disk1, disk3, dist13 });
			m_links.push_back({ disk2, disk3, dist23 });
		}
		
	}

	void applyLinks()
	{
		for (auto& link : m_links)
		{
			link.apply();
		}
	}

private:
	std::vector<RigidDisk*> m_components;
	std::vector<Link> m_links;
};

#endif