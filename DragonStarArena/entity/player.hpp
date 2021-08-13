// ================================================================
//
// player.hpp
//
// Class for the player characters.
//
// ================================================================

#pragma once

struct RaceData;

#include <array>
#include "actor.hpp"
#include "../data/item.hpp"
#include "../data/statMod.hpp"

class Player : public Actor {
public:
	Player();
	Player(std::string name, size_t raceID, BattlePosition p);

	bool IsPlayer();

	int64_t GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs);
private:
	int64_t getBaseStat(StatModType statModType);
	std::vector<StatMod> getBaseStatMods();
	int64_t getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions);

	// ================================

	RaceData* raceData = nullptr;
	int currentEXP = 0;

	// Slot  0: Main-Hand
	// Slot  1: Off-Hand
	// Slot  2: Head
	// Slot  3: Body
	// Slot  4: Hands
	// Slot  5: Legs
	// Slot  6: Feet
	// Slot  7: Belt
	// Slot  8: Neck
	// Slot  9: Ring 1
	// Slot 10: Ring 2
	// Slot 11: Trinket
	std::array<Item, 12> equipment;
};