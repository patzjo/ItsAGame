#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

struct PlayerKeys
{
	sf::Keyboard::Key key;
	std::string name;
};

struct GameOptions
{
	int screenWidth  = 1920;
	int screenHeight = 1080;
	int levelWidth	 = 1920;
	int levelHeight  = 1080;
	int players = 2;

	int maxPlayers = 4;

	PlayerKeys playerKeys[4][5] =
	{
		{	// P1
			sf::Keyboard::Key::Left, "Move Left",
			sf::Keyboard::Key::Right, "Move Right",
			sf::Keyboard::Key::RControl, "Fire Cannon",
			sf::Keyboard::Key::Down, "Move Cannon Counterclockwise",
			sf::Keyboard::Key::Up, "Move Cannon Clockwise"
		},
		
		{	// P2
			sf::Keyboard::Key::A, "Move Left",
			sf::Keyboard::Key::D, "Move Right",
			sf::Keyboard::Key::LControl, "Fire Cannon",
			sf::Keyboard::Key::W, "Move Cannon Counterclockwise",
			sf::Keyboard::Key::S, "Move Cannon Clockwise"
		},

		{	// P3
			sf::Keyboard::Key::F, "Move Left",
			sf::Keyboard::Key::H, "Move Right",
			sf::Keyboard::Key::X, "Fire Cannon",
			sf::Keyboard::Key::T, "Move Cannon Counterclockwise",
			sf::Keyboard::Key::G, "Move Cannon Clockwise"
		},

		{	// P4
			sf::Keyboard::Key::J, "Move Left",
			sf::Keyboard::Key::L, "Move Right",
			sf::Keyboard::Key::B, "Fire Cannon",
			sf::Keyboard::Key::I, "Move Cannon Counterclockwise",
			sf::Keyboard::Key::K, "Move Cannon Clockwise"
		}

	};


};