#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <numeric>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <utility>

namespace Utility
{
	constexpr std::pair<sf::Color, sf::Color> honey		{ { 255, 192, 0 }, {139, 64, 0} };
	constexpr std::pair<sf::Color, sf::Color> orange	{ {255, 172, 28}, {204, 85, 0 } };
	inline float average(std::vector<float> const& v){

		if(v.empty()){
			return 0;
		}

		auto const count = static_cast<float>(v.size());
		return std::reduce(v.begin(), v.end()) / count;
	}
	
	inline float distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return (p1 - p2).lengthSquared();
	}
	inline float distance(const sf::Vector2i& p1, const sf::Vector2f& p2)
	{
		return (static_cast<sf::Vector2f>(p1) - p2).lengthSquared();
	}
	inline float distance(const sf::Vector2f& p1, const sf::Vector2i& p2)
	{
		return (p1 - static_cast<sf::Vector2f>(p2)).lengthSquared();
	}
	inline float distance(const sf::Vector2i& p1, const sf::Vector2i& p2)
	{
		return (p1 - p2).lengthSquared();
	}

	template <typename T> 
	int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	inline const sf::Color getGradient(const float t, const sf::Color& c1, const sf::Color& c2)
	{
		const float r{ c1.r + std::sin(t) * std::sin(t) * (c2.r - c1.r) };
		const float g{ c1.g + std::sin(t) * std::sin(t) * (c2.g - c1.g) };
		const float b{ c1.b + std::sin(t) * std::sin(t) * (c2.b - c1.b) };

		return { static_cast<uint8_t>(r), static_cast<uint8_t>(g) ,static_cast<uint8_t>(b) };
	}

}

#endif