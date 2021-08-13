// ================================================================
//
// dungeonMapScene.hpp
// 
// Scene where battles take place.
//
// ================================================================

#pragma once

enum class GameState;

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../entity/player.hpp"
#include "../entity/encounterNode.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class DungeonMapScene {
public:
	DungeonMapScene();

	// Reads input.
	void ReadInput(sf::RenderWindow& window);

	// Updates the scene and returns the game state.
	GameState Update(float secondsPerUpdate);

	// Draws the scene.
	void Render(sf::RenderTarget& window, float timeRatio);

	// Creates the party based on the given data.
	// todo: character creation
	void CreateParty();

	// Creates encounter nodes with a fixed test seed.
	void GenerateEncounterNodes();

	// Creates encounter nodes based on the given seed.
	void GenerateEncounterNodes(uint64_t seed);

private:
	// Returns the average level of the party, rounded down.
	int getAveragePartyLevel();
	
	// ================================

	// Party
	std::vector<ActorPtr> party{};
	int64_t gold = 0;
	int64_t dust = 0;

	// Encounters
	uint64_t masterSeed = 0xdeadbeefdeadbeef;
	std::vector<EncounterNode> encounterNodes{};
	EncounterNode* choosenEncounter = nullptr;

	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;
};
