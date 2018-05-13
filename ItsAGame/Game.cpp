#include <iomanip>
#include <sstream>
#include "GameOptions.h"
#include "World.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Noise.h"
#include "Obspat.h"
#include "Particles.h"
#include "Level.h"
#include "Command.h"
#include "Input.h"
#include "Game.h"

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vSync, bool fullScreen)
{
	renderer.initialize(ScreenWidth, ScreenHeight, name, vSync, fullScreen);
	world.setGamePointer(this);
	
	renderer.setWorldPointer(&world);
	renderer.setLevelPointer(&world.level);

	world.initialize();
	sound.initialize();
}

void Game::run()
{
	sf::Text text;
	
	int frames = 0;
	int framesPerSec = 0;
	float timeElapsed = 0.0f;
	bool firstFrame = false;
	float winningInformationTime = 0.0f;
	float blinky = 0.0f;
	sf::Clock clock;
	while (state.getState() != StateEnum::QUIT)
	{
		deltaTime = clock.restart().asSeconds();
		state.changeRequestedState();
		processEvents(renderer.getWindow());

		switch (state.getState())
		{
		case StateEnum::MENU:
			menuLoop(deltaTime);
			break;

		case StateEnum::START_NEW_GAME:
			world.notifySubject(E_START_AGAIN, 0);
			world.level.generateRectangleLevel(options.screenWidth, options.screenHeight, 200, 600, 100, 200, sf::Color::Blue);
			world.collisionTree.setLevelCollisionBoxes(world.level.getLevelCollisionBoxes());
			world.startAgain();
			winningInformationTime = 0.0f;
			state.requestState(StateEnum::PLAYING);
			world.applyWind(4, 4);
			firstFrame = true;
		break;


		case StateEnum::ANNOUNCE_WINNER:
		{
			winningInformationTime += deltaTime;
			if (winningInformationTime > 10.0f)
			{
				winningInformationTime = 0.0f;
				state.requestState(START_NEW_GAME);
			}

			renderer.pushText("Winner is " + roundWinner, {renderer.getWindow().getSize().x/2.0f, renderer.getWindow().getSize().y / 2.0f - 200.0f}, 0, 128, sf::Color::Yellow, true);
			renderer.pushText("New round in " + std::to_string((int)(10.0f - winningInformationTime)), { renderer.getWindow().getSize().x / 2.0f, renderer.getWindow().getSize().y / 2.0f }, 0, 128, sf::Color::Yellow, true);
		} 

		case StateEnum::PLAYING:
			playLoop(deltaTime);
			break;

		default:break;
		}

		
		if (!firstFrame)
		{
			if (deltaTime > deltaTimeMax)
				deltaTimeMax = deltaTime;

			if (deltaTime < deltaTimeMin)
				deltaTimeMin = deltaTime;
		}

		// Update frames per seconds
		timeElapsed += deltaTime;
		frames++;
		if (timeElapsed >= 1.0)
		{
			timeElapsed = 0.0f;
			framesPerSec = frames;
			frames = 0;
		}


		showFPS(framesPerSec);
		
		renderer.render();
		
		world.processQueues();

		firstFrame = false;
		blinky+=0.1f;
	}

	renderer.shutDown();
}

void Game::processEvents(sf::RenderWindow & window)
{
	input.updateMousePosition(sf::Mouse::getPosition(renderer.getWindow()));
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			state.setState(StateEnum::QUIT);
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				if (state.getState() != StateEnum::MENU)
					state.setState(StateEnum::MENU);
				else
					state.setState(StateEnum::QUIT);
				break;

			case sf::Keyboard::I:
				world.createObject((sf::Vector2f)input.getMousePosition(), new CannonBall());
				break;

			case sf::Keyboard::Q:
				world.collisionTree.toggleDebug();
				break;

			case sf::Keyboard::F1:
				state.requestState(StateEnum::START_NEW_GAME);
				break;


			default: break;
			}
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Return:
				if (state.getState() == StateEnum::MENU)
				{
					std::string selection = menu.getSelection();
					if (selection == "Start Game")
						state.requestState(StateEnum::START_NEW_GAME);

					if (selection == "Quit Game")
						state.requestState(StateEnum::QUIT);
				}
				break;

			case sf::Keyboard::Up:
				if (state.getState() == StateEnum::MENU)
					menu.selectionUp();
				break;

			case sf::Keyboard::Down:
				if (state.getState() == StateEnum::MENU)
					menu.selectionDown();
				break;

			default: break;
			}
		}
	}

	// Do not update keys if winner is clear
	if ( state.getState() != ANNOUNCE_WINNER )
		for (unsigned int c = 0; c < playerCount; c++)
		{
			if ( players[c] )
				if ( players[c]->active )
					input.processInput(this, players[c]);
		}

}

void Game::playLoop( float deltaTime )
{
	std::string windString("Wind: ");
	windString += to_string_with_precision(world.getWind().x, 2);
	renderer.pushText(windString, { renderer.getWindow().getSize().x / 2.0f, 10 }, 0, 32, sf::Color::White, true);

	world.update(deltaTime);
	renderer.updateAnimations(deltaTime);
}

void Game::menuLoop(float deltaTime)
{
	menu.update(deltaTime);
	menu.render( &renderer );

}

void Game::showFPS(int frameCount)
{
	if (!FPSCounter)
		return;
	
	std::string text;
	text = "FPS: ";
	text += std::to_string(frameCount);

	text += " Objects: ";
	text += std::to_string(world.gameObjects.size());

	text += " DeltaTimeMin: ";
	text += std::to_string(deltaTimeMin);

	text += " DeltaTimeMax: ";
	text += std::to_string(deltaTimeMax);
/*
	text += " Falling: ";
	text += players[0]->falling ? "Yes" : "No";
*/
	renderer.pushText(text, { 0.0f, 0.0f }, 0, 32, sf::Color::White);
}

void Game::updateGameSituation(PlayerKilledData * data)
{
	int alivePlayers = 0;
	for (unsigned int c = 0; c < playerCount; c++)
		if (players[c])
		{
			roundWinner = players[c]->getName();
			alivePlayers++;
		}

	if (alivePlayers <= 1 && state.getState() == StateEnum::PLAYING)
	{
		state.requestState(ANNOUNCE_WINNER);
		//		state.setState(START_NEW_GAME);
	}
}

