#ifndef RENDERER_H
#define RENDERER_H

#include "SFML/Graphics.hpp"

#include "physics/solver.h"

class Renderer
{
public:
	Renderer() = delete;
	Renderer(const Solver& solver) : m_solver {solver}, m_vertices{sf::PrimitiveType::Triangles}
	{
		if (!m_texture.loadFromFile("res/circle.png")) return;
		m_texture.setSmooth(true);
	}

	void render(sf::RenderTarget& target);

	void updateVertices();

private:
	const Solver& m_solver;

	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};

#endif