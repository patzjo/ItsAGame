#include <iostream>
#include "Noise.h"
#include "Assets.h"


Noise::Noise()
{
}


Noise::~Noise()
{
}

bool Noise::initialize()
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
				std::cout << "Error loading sound file: " << soundsPath + assets[c].filename << "." << std::endl;
			else
				sfxs.push_back(buffer);
			break;
		case AssetType::MUSIC:
			musics.push_back(musicsPath + assets[c].filename);
			break;

		default: break;
		}
	}
	return false;
}

void Noise::playMusic(int index)
{
	music.openFromFile(musics[index]);
	music.play();
}

void Noise::playSound(int index)
{
	for (unsigned int c = 0; c < sizeof(channel) / sizeof(sf::Sound); c++)
	{
		if (channel[c].getStatus() != sf::Sound::Status::Playing)
		{
			channel[c].setBuffer(sfxs[index]);
			channel[c].play();

			break;
		}
	}
}
