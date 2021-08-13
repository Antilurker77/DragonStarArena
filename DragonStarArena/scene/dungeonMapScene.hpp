// ================================================================
//
// dungeonMapScene.hpp
// 
// Scene where battles take place.
//
// ================================================================

#pragma once

enum class GameState;

#include <SFML/Graphics.hpp>

class DungeonMapScene {
public:
	DungeonMapScene();

	// Reads input.
	void ReadInput(sf::RenderWindow& window);

	// Updates the scene and returns the game state.
	GameState Update(float secondsPerUpdate);

	// Draws the scene.
	void Render(sf::RenderTarget& window, float timeRatio);

private:

	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;
};
