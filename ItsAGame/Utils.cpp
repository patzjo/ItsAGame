#include <SFML/Graphics.hpp>
#include "Utils.h"
#include <iostream>

bool collisionBetweenCircles(sf::Vector2f c1pos, float c1Radius, sf::Vector2f c2pos, float c2Radius)
{
	float rangeX = abs(c2pos.x - c1pos.x);
	float rangeY = abs(c2pos.y - c1pos.y);

	float squareC1Radius = c1Radius * c1Radius;
	float squareC2Radius = c2Radius * c2Radius;

	if ((rangeX*rangeX + rangeY*rangeY) <= squareC1Radius + squareC2Radius)
		return true;

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

std::ostream & operator<<(std::ostream & o, sf::Vector2f & r)
{
	o << "(" << r.x << ", " << r.y << ")";
	return o;
}

float toRad(float degrees)
{
	float radians = degrees * (PI/180.0f);
	return radians;
}
