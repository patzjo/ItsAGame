#pragma once
#include "GameOptions.h"

class Menu
{
public:
	void update(float dT);
	void render(class Renderer * renderer);

	void selectionDown();
	void selectionUp();

	std::string getSelection();

private:
	void saveSettings( GameOptions *gameOptions );
	void loadSettings( GameOptions *gameOptions );
	GameOptions options;

	int selected = 1;
};