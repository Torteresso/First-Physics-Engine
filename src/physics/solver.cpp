#include "physics/solver.h"
#include <iostream>
#include <cmath>


void Solver::update(const float dt)
{
	const int nbSubStep{ 8 };
	const float subDt{ dt / static_cast<float>(nbSubStep) };
	for (int i{}; i < nbSubStep; i++)
	{
		updatePos(subDt);
		applyConstraints();
		solveCollision();
	}
}

void Solver::solveCollision()
{
	for (int i {}; i < m_objects.size(); i++)
	{
		auto& obj1{ m_objects[i] };
		for (int j{ i + 1 }; j < m_objects.size(); j++)
		{
			auto& obj2{ m_objects[j] };

			sf::Vector2f n{ obj2.pos - obj1.pos };

			const float delta = n.length() - obj1.radius - obj2.radius;

			if (delta > 0) continue;

			if (n.length() < 1e-6) n = sf::Vector2f{ 1.f, 1.f };

			obj1.pos += 0.5f * delta * n.normalized();
			obj2.pos -= 0.5f * delta * n.normalized();
		}
	}
}

void Solver::updatePos(const float dt)
{
	for (auto& obj : m_objects)
	{
		sf::Vector2f newOldPos{ obj.pos };
		obj.pos += obj.pos - obj.oldPos + Config::gravity * dt * dt;
		obj.oldPos = newOldPos;
	}
}

void Solver::applyConstraints()
{
	for (auto& obj : m_objects)
	{
		if (obj.pos.y + obj.radius >= Config::windowSizef.y) obj.pos.y = Config::windowSizef.y - obj.radius;
		if (obj.pos.y - obj.radius <= 0) obj.pos.y = obj.radius;
		if (obj.pos.x + obj.radius >= Config::windowSizef.x) obj.pos.x = Config::windowSizef.x - obj.radius;
		if (obj.pos.x - obj.radius <= 0) obj.pos.x = obj.radius;
	}
}


void Solver::addObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, const sf::Color& color)
{
	m_objects.push_back({ radius, pos, oldPos, color });
}
