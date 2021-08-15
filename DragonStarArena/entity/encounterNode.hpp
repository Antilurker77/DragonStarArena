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

typedef std::pair<size_t, size_t> Location;

enum class EncounterType {
	Undefined = 0,
	Battle,
	UniqueBattle,
	Treasure
};

class EncounterNode : public Entity {
public:
	EncounterNode();
	EncounterNode(sf::Vector2f spawnPos, size_t xLoc, size_t yLoc, int level);

	// Returns the location in the grid of this node.
	Location GetLocation();

private:
	EncounterType encounterType{};
	EncounterData* encounterData = nullptr;
	Location location{};
};
