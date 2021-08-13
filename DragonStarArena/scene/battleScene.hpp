// ================================================================
//
// battleScene.hpp
// 
// Scene where battles take place.
//
// ================================================================

#pragma once

class Actor;
enum class BattlePosition;
enum class GameState;

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../entity/actor.hpp"
#include "../ui/richText.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class BattleScene {
public:
	BattleScene();

	// Initializes a mock battle.
	void InitTestBattle();

	// Reads input.
	void ReadInput(sf::RenderWindow& window);

	// Updates the scene and returns the game state.
	GameState Update(float secondsPerUpdate);

	// Draws the scene.
	void Render(sf::RenderTarget& window, float timeRatio);

	// Returns a pointer to a player or monster at the designated
	// battle position. Returns nullptr if no one is there.
	Actor* GetActorAtPosition(BattlePosition battlePosition, bool isPlayer);

	// Adds a message to the message log.
	void AddMessage(std::string message);

	// Clears the message log.
	void ClearMessageLog();

private:
	// Sets the positions of the actor sprites.
	void setActorSpritePositions();

	// ================================

	std::vector<ActorPtr> actors{};
	int timePassed = 0;

	// Debug
	sf::Text playerText;
	sf::Text enemyText;

	// UI
	sf::RectangleShape messageLogBG;
	std::vector<sfe::RichText> messageLog{};
	const unsigned int messageLogFontSize = 16u;
	const size_t messagesDisplayed = 20ull;

	std::array<sf::RectangleShape, 20> actorBoxes;

	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;
};