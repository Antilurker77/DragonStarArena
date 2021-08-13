// ================================================================
//
// encounterNode.cpp
//
// ================================================================

#include "encounterNode.hpp"

#include "../core/assetManager.hpp"
#include "../data/enconterData.hpp"
#include "../data/gameData.hpp"

EncounterNode::EncounterNode() {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/node/test_node.png"));
}

EncounterNode::EncounterNode(sf::Vector2f spawnPos, int level) {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/node/test_node.png"));
	auto spriteSize = sprites[0].getLocalBounds();

	sprites[0].setPosition(spawnPos.x - spriteSize.width, spawnPos.y - spriteSize.height);

	// todo: select random encounter
	encounterData = gameData.GetEncounter(3);
}