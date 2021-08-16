// ================================================================
//
// encounterNode.cpp
//
// ================================================================

#include "encounterNode.hpp"

#include "../core/assetManager.hpp"
#include "../data/enconterData.hpp"
#include "../data/gameData.hpp"
#include "../data/id/encounterType.hpp"

EncounterNode::EncounterNode() {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/node/test_node.png"));
}

EncounterNode::EncounterNode(sf::Vector2f spawnPos, size_t xLoc, size_t yLoc, int level, std::mt19937_64& mt) {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/node/test_node.png"));
	auto spriteSize = sprites[0].getLocalBounds();

	sprites[0].setPosition(spawnPos.x - spriteSize.width, spawnPos.y - spriteSize.height);

	location.first = xLoc;
	location.second = yLoc;

	generateEncounter(level, mt);
}

void EncounterNode::SetBossNode(int level, std::mt19937_64& mt) {
	encounterType = EncounterType::UniqueBattle;
	encounterData = gameData.GetEncounterRandom(level, mt);
}

Location EncounterNode::GetLocation() {
	return location;
}

EncounterType EncounterNode::GetEncounterType() {
	return encounterType;
}

void EncounterNode::generateEncounter(int level, std::mt19937_64& mt) {
	encounterType = gameData.GetEncounterTypeRandom(mt);

	if (encounterType == EncounterType::Battle) {
		encounterData = gameData.GetEncounterRandom(level, mt);
	}
}