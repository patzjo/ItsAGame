#pragma once
#include <SFML/Audio.hpp>

class Sound
{
public:
	Sound();
	~Sound();

	bool initialize();

private:
	sf::Sound sfx;
	sf::Music music;
};

