// ================================================================
//
// encounterNode.hpp
// 
// Instance of a single encounter.
//
// ================================================================

#pragma once

struct EncounterData;
enum class EncounterType;

#include <random>
#include <SFML/System.hpp>
#include "entity.hpp"

typedef std::pair<size_t, size_t> Location;

class EncounterNode : public Entity {
public:
	EncounterNode();
	EncounterNode(sf::Vector2f spawnPos, size_t xLoc, size_t yLoc, int level, std::mt19937_64& mt);

	// Turns the node into a boss node.
	void SetBossNode(int level, std::mt19937_64& mt);

	// Returns the location in the grid of this node.
	Location GetLocation();

	// Returns the encounter type of this node.
	EncounterType GetEncounterType();

private:
	// Selects what kind of encounter this node should be.
	void generateEncounter(int level, std::mt19937_64& mt);
	
	// ================================
	
	EncounterType encounterType{};
	EncounterData* encounterData = nullptr;
	Location location{};
};
