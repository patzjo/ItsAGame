#include <iostream>
#include "Sound.h"
#include "Assets.h"


Sound::Sound()
{
}


Sound::~Sound()
{
}

bool Sound::initialize()
{
	sf::SoundBuffer buffer;
	std::string soundsPath;
	std::string musicsPath;
	soundsPath = musicsPath = assetsDirectory;
	soundsPath += soundsDirectory;
	musicsPath += musicsDirectory;

	for (unsigned int c = 0; c < sizeof(assets) / sizeof(assets[0]); c++)
	{
		switch (assets[c].type)
		{
		case AssetType::SOUND:
			if (!buffer.loadFromFile(soundsPath + assets[c].filename))
				std::cout << "Error loading sound file." << std::endl;
			break;
		case AssetType::MUSIC:
			if (!buffer.loadFromFile(musicsPath + assets[c].filename))
				std::cout << "Error loading music file." << std::endl;
			break;

		default: break;
		}
	}
	return false;
}

void Sound::playMusic(int index)
{

}

void Sound::playSound(int index)
{
	for (unsigned int c = 0; c < sizeof(channel) / sizeof(sf::Sound); c++)
	{
		if (channel[c].getStatus() == sf::Sound::Status::Stopped)
		{
			channel[c].setBuffer(sfxs[index]);
			channel[c].play();
		}
	}
}
