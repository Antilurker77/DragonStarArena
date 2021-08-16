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
typedef std::pair<size_t, size_t> Connection;
typedef std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> LocationPair;

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

	// Builds the vertex array for the nodes.
	void BuildNodeVertexArray();

	// Returns the party vector.
	std::vector<ActorPtr> GetParty();

	// Returns the encounter node the user has selected.
	EncounterNode* GetChoosenEncounter();

private:
	// Returns the average level of the party, rounded down.
	int getAveragePartyLevel();
	
	// Builds the vertex array for connection lines.
	void buildLineVertexArray();

	// ================================

	// Party
	std::vector<ActorPtr> party{};
	int64_t gold = 0;
	int64_t dust = 0;

	// Encounters
	uint64_t masterSeed = 0xdeadbeefdeadbeef;
	std::vector<EncounterNode> encounterNodes{};
	std::vector<Connection> connections{};
	std::vector<sf::RectangleShape> nodeBoxes{};
	sf::VertexArray lines;
	sf::VertexArray nodes;
	sf::Texture* nodeTexture = nullptr;
	EncounterNode* choosenEncounter = nullptr;

	std::vector<size_t> availableNodes{};
	sf::Vector2<size_t> currentLocation = { 0, 0 };
	bool hasStarted = false;

	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;
};
