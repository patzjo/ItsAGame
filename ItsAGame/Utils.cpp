#include <SFML/Graphics.hpp>
#include "Utils.h"
#include <iostream>
bool collisionBetweenCircleAndRectangle(sf::Vector2f cPointOfSquareObject, Rectangle<float> &l, sf::Vector2f cPointOfCircleObject, float radius)
{
	float circleDistanceX = abs(cPointOfCircleObject.x - cPointOfSquareObject.x);
	float circleDistanceY = abs(cPointOfCircleObject.y - cPointOfSquareObject.y);

	if (circleDistanceX > (l.halfSize.x + radius)) { return false; }
	if (circleDistanceY > (l.halfSize.y + radius)) { return false; }

	if (circleDistanceX <= l.halfSize.x) { return true; }
	if (circleDistanceY <= l.halfSize.y) { return true; }

	float cornerDistance_sq = (circleDistanceX - l.halfSize.x)*(circleDistanceX - l.halfSize.x)
		+ (circleDistanceY - l.halfSize.y)*(circleDistanceY - l.halfSize.y);

	return (cornerDistance_sq <= (radius*radius));
}

bool collisionBetweenCircles(sf::Vector2f c1pos, float c1radius, sf::Vector2f c2pos, float c2radius)
{
	float rangeX = abs(c2pos.x - c1pos.x);
	float rangeY = abs(c2pos.y - c1pos.y);

	if ((rangeX*rangeX + rangeY * rangeY) <= c1radius + c2radius)
	{
		return true;
	}

	return false;
}


sf::Vector2f operator+(sf::Vector2f& l, sf::Vector2f& r)
{
	return sf::Vector2f(l.x + r.x, l.y + r.y);
}

sf::Vector2f &operator +=(sf::Vector2f& l, sf::Vector2f& r)
{
	l.x += r.x;
	l.y += r.y;

	return l;
}

sf::Vector2f operator-(sf::Vector2f& l, sf::Vector2f& r)
{
	return sf::Vector2f(l.x - r.x, l.y - r.y);
}

sf::Vector2f &operator -=(sf::Vector2f& l, sf::Vector2f& r)
{
	l.x -= r.x;
	l.y -= r.y;

	return l;
}

sf::Vector2f operator*(sf::Vector2f& l, float scalar)
{
	sf::Vector2f result(l.x * scalar, l.y * scalar);
	return result;
}

sf::Vector2f & operator*=(sf::Vector2f & l, float scalar)
{
	l.x *= scalar;
	l.y *= scalar;
	return l;
}

sf::Vector2f operator*(float scalar, sf::Vector2f& r)
{
	return r * scalar;
}
