// ================================================================
//
// encounterNode.hpp
// 
// Instance of a single encounter.
//
// ================================================================

#pragma once

struct EncounterData;

#include <SFML/System.hpp>
#include "entity.hpp"

enum class EncounterType {
	Undefined = 0,
	Battle,
	UniqueBattle,
	Treasure
};

class EncounterNode : public Entity {
public:
	EncounterNode();
	EncounterNode(sf::Vector2f spawnPos, int level);

private:
	EncounterType encounterType{};
	EncounterData* encounterData = nullptr;
};
