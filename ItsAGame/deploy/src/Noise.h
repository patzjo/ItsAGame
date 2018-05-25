#pragma once
#include <SFML/Audio.hpp>

class Noise
{
public:
	Noise();
	~Noise();

	bool initialize();

	void playMusic(int index);
	void playSound(int index);

private:
	std::vector <sf::SoundBuffer> sfxs;
	std::vector <std::string> musics;

	sf::Sound channel[32];
	sf::Music music;
};

