#pragma once
#include <ostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ostream>

sf::Vector2f operator+(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f &operator +=(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f operator-(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f &operator -=(sf::Vector2f& l, sf::Vector2f& r);
sf::Vector2f operator*(sf::Vector2f& l, float scalar);
sf::Vector2f& operator*=(sf::Vector2f& l, float scalar);
sf::Vector2f operator*(float scalar, sf::Vector2f& r);
std::ostream& operator <<(std::ostream& o, sf::Vector2f& r);


template <typename T>
struct Rectangle
{
	sf::Vector2<T> centerPos;
	sf::Vector2<T> halfSize;

	Rectangle(const sf::Vector2<T>& CenterPos, const sf::Vector2<T>& HalfSize)
	{
		centerPos = CenterPos;
		halfSize = HalfSize;
	}

	Rectangle() {
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

	bool boxIntersect(const Rectangle<T>& other)
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

	bool circleIntersect(sf::Vector2f cPointOfCircleObject, float radius)
	{
		float distanceX = abs(cPointOfCircleObject.x - centerPos.x);
		float distanceY = abs(cPointOfCircleObject.y - centerPos.y);

		if (distanceX > (halfSize.x + radius)) { return false; }
		if (distanceY > (halfSize.y + radius)) { return false; }

		if (distanceX <= halfSize.x) { return true; }
		if (distanceY <= halfSize.y) { return true; }

		float cornerDistance_sq = (distanceX - halfSize.x)*(distanceX - halfSize.x)
			+ (distanceY - halfSize.y)*(distanceY - halfSize.y);

		return (cornerDistance_sq <= (radius*radius));
	}

};


bool collisionBetweenCircleAndRectangle(sf::Vector2f cPointOfSquareObject, Rectangle<float> &l, sf::Vector2f cPointOfCircleObject, float radius);
bool collisionBetweenCircles(sf::Vector2f c1pos, float c1radius, sf::Vector2f c2pos, float c2radius);
