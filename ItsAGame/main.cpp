#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"

using std::endl;
using std::cout;



int main(int argc, char **argv)
{
	Game game;

	game.initialize(1920, 1080, "ItsAGame!");
	game.run();

	return 0;
}
