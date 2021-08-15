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

EncounterNode::EncounterNode(sf::Vector2f spawnPos, size_t xLoc, size_t yLoc, int level) {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/node/test_node.png"));
	auto spriteSize = sprites[0].getLocalBounds();

	sprites[0].setPosition(spawnPos.x - spriteSize.width, spawnPos.y - spriteSize.height);

	location.first = xLoc;
	location.second = yLoc;

	// todo: select random encounter
	encounterData = gameData.GetEncounter(3);
}

Location EncounterNode::GetLocation() {
	return location;
}