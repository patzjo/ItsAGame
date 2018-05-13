#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Menu.h"


struct MenuItem
{
	std::string name;
	int fontID = 0;
	int characterSize = 20;
	sf::Color color;
	bool canSelect = true;
};

MenuItem menuItems[]
{
	"[ ItsAGame ]", 0, 64, sf::Color::Yellow, false,
	"Start Game", 0, 32,   sf::Color::Yellow, true,
	"Quit Game", 0, 32,    sf::Color::Yellow, true,
};

void Menu::update(float dT)
{

}

void Menu::render(Renderer *renderer)
{
	sf::RenderWindow &w = renderer->getWindow();
	float halfWith = renderer->getWindow().getSize().x / 2.0f;

	float yPosition = 100.0f;
	for (unsigned int c = 0; c < sizeof(menuItems) / sizeof(MenuItem); c++)
	{
		MenuItem &item = menuItems[c];
		
		if (c == selected)
		{
			std::string name = "-> ";
			name += item.name;
			name += " <-";
			renderer->pushText(name, { halfWith, yPosition }, item.fontID, item.characterSize, item.color, true);
		}
		else
		{
			renderer->pushText(item.name, { halfWith, yPosition }, item.fontID, item.characterSize, item.color, true);
		}

		if (item.canSelect)
			yPosition += item.characterSize + 5.0f;
		else
			yPosition += item.characterSize * 2.0f;
	}
}

void Menu::selectionDown()
{
	for (unsigned int c = selected+1; c < sizeof(menuItems) / sizeof(MenuItem); c++)
	{
		if (menuItems[c].canSelect)
		{
			selected = c;
			return;
		}
	}

	for (unsigned int c = 0; c < sizeof(menuItems) / sizeof(MenuItem); c++)
	{
		if (menuItems[c].canSelect)
		{
			selected = c;
			return;
		}
	}
}

void Menu::selectionUp()
{
	for (unsigned int c = selected - 1; c > 0; c--)
	{
		if (menuItems[c].canSelect)
		{
			selected = c;
			return;
		}
	}

	for (unsigned int c = sizeof(menuItems) / sizeof(MenuItem); c > 0 ; c--)
	{
		if (menuItems[c].canSelect)
		{
			selected = c;
			return;
		}
	}
}

std::string Menu::getSelection()
{
	return menuItems[selected].name;
}

void Menu::saveSettings(GameOptions * gameOptions)
{
	std::fstream file;
	file.open("settings.bin", std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		file.write((char *)&gameOptions, sizeof(GameOptions));
		file.close();
	}
}

void Menu::loadSettings(GameOptions * gameOptions)
{
	std::fstream file;
	file.open("settings.bin", std::ios::out | std::ios::binary);
	
	if (file.is_open())
	{
		file.read((char *)&gameOptions, sizeof(GameOptions));
		file.close();
	}
	
}
