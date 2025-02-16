#include "renderer/renderer.h"

void Renderer::render(sf::RenderTarget& target)
{
	updateVertices();
	if (Config::debug) debug(target);

	target.draw(m_vertices, &m_texture);
}

void Renderer::updateVertices()
{
	const auto& disks{ m_solver.m_disks };

	const sf::Vector2f& textureSize{ static_cast<sf::Vector2f>(m_texture.getSize()) };

	m_vertices.resize(disks.size() * 6);
	for (int i{}; i < disks.size(); i++)
	{
		const auto& disk{ *disks[i] };
		const auto& pos{ disk.pos };
		const auto& color{ disk.color };
		const float radius{ disk.radius };

		m_vertices[6 * i + 0].position = pos + sf::Vector2(-radius, -radius);
		m_vertices[6 * i + 1].position = pos + sf::Vector2(radius, -radius);
		m_vertices[6 * i + 2].position = pos + sf::Vector2(radius, radius);
		m_vertices[6 * i + 3].position = pos + sf::Vector2(-radius, -radius);
		m_vertices[6 * i + 4].position = pos + sf::Vector2(-radius, radius);
		m_vertices[6 * i + 5].position = pos + sf::Vector2(radius, radius);

		m_vertices[6 * i + 0].color = color;
		m_vertices[6 * i + 1].color = color;
		m_vertices[6 * i + 2].color = color;
		m_vertices[6 * i + 3].color = color;
		m_vertices[6 * i + 4].color = color;
		m_vertices[6 * i + 5].color = color;

		m_vertices[6 * i + 0].texCoords = { 0.f, 0.f };
		m_vertices[6 * i + 1].texCoords = { textureSize.x, 0.f };
		m_vertices[6 * i + 2].texCoords = textureSize;
		m_vertices[6 * i + 3].texCoords = { 0.f, 0.f };
		m_vertices[6 * i + 4].texCoords = { 0.f, textureSize.y };
		m_vertices[6 * i + 5].texCoords = textureSize;
	}
}

void Renderer::debug(sf::RenderTarget& target)
{
	m_solver.updateDebug();
	if (m_solver.m_disks.size() < 1) return;

	m_vertices[6 * m_solver.m_diskViewed + 0].color = sf::Color::Green;
	m_vertices[6 * m_solver.m_diskViewed + 1].color = sf::Color::Green;
	m_vertices[6 * m_solver.m_diskViewed + 2].color = sf::Color::Green;
	m_vertices[6 * m_solver.m_diskViewed + 3].color = sf::Color::Green;
	m_vertices[6 * m_solver.m_diskViewed + 4].color = sf::Color::Green;
	m_vertices[6 * m_solver.m_diskViewed + 5].color = sf::Color::Green;

	sf::CircleShape oldPosCircle{ Config::diskRadius };
	oldPosCircle.setPosition(m_solver.m_disks[m_solver.m_diskViewed]->oldPos - sf::Vector2f{Config::diskRadius, Config::diskRadius});
	oldPosCircle.setOutlineColor(sf::Color::Red);
	oldPosCircle.setOutlineThickness(1.f);
	target.draw(oldPosCircle);
}