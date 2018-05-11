#pragma once
#include <SFML/Audio.hpp>

class Sound
{
public:
	Sound();
	~Sound();

	bool initialize();

	void playMusic(int index);
	void playSound(int index);

private:
	std::vector <sf::SoundBuffer> sfxs;
	std::vector <sf::SoundBuffer> musics;

	sf::Sound channel[32];
	sf::Music music;
};

