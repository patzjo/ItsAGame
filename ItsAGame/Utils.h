#pragma once
#include <ostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

sf::Vector2f operator+(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f &operator +=(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f operator-(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f &operator -=(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f operator*(sf::Vector2f& l, float scalar);
sf::Vector2f operator*(float scalar, sf::Vector2f& r);

template <typename T>
struct Square
{
	sf::Vector2<T> centerPos;
	sf::Vector2<T> halfSize;

	Square(const sf::Vector2<T>& CenterPos, const sf::Vector2<T>& HalfSize)
	{
		centerPos = CenterPos;
		halfSize = HalfSize;
	}

	Square() {
		centerPos = { 0, 0 };
		halfSize = { 0, 0 };
	}

	bool containsPoint(const sf::Vector2<T>& pos)
	{
		T minX = centerPos.x - halfSize.x;
		T minY = centerPos.y - halfSize.y;

		T maxX = centerPos.x + halfSize.x;
		T maxY = centerPos.y + halfSize.y;

		return pos.x <= maxX &&
			pos.x >= minX &&
			pos.y <= maxY &&
			pos.y >= minY;
	}

	bool boxIntersect(const Square<T>& other)
	{
		sf::Vector2f aTL(centerPos.x - halfSize.x, centerPos.y - halfSize.y);
		sf::Vector2f aBR(centerPos.x + halfSize.x, centerPos.y + halfSize.y);

		sf::Vector2f bTL(other.centerPos.x - other.halfSize.x, other.centerPos.y - other.halfSize.y);
		sf::Vector2f bBR(other.centerPos.x + other.halfSize.x, other.centerPos.y + other.halfSize.y);

		if (aBR.x < bTL.x ||
			aTL.x > bBR.x ||
			aBR.y < bTL.y ||
			aTL.y > bBR.y)
			return false;

		return true;
	}


};


