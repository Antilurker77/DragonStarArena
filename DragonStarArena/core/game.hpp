// ================================================================
//
// game.hpp
// 
// Handles the core game loop.
//
// ================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "gameState.hpp"
#include "../scene/battleScene.hpp"
#include "../scene/mainMenuScene.hpp"
#include "../ui/fpsWindow.hpp"

class Game {
public:
	Game();

	// Runs the game.
	void Run();

private:
	// Reads player input.
	void processInput();

	// Updates game logic.
	void update(float secondsPerUpdate);

	// Draws the game to the window.
	void draw(float timeRatio);

	// ================================

	sf::RenderWindow window;

	GameState gameState{};
	MainMenuScene mainMenuScene;
	BattleScene battleScene;

	FPSWindow fpsWindow;
};