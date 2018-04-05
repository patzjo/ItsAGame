#include <SFML/Graphics.hpp>
#include "Utils.h"

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
