// ================================================================
//
// gameData.hpp
// 
// Global that holds all game data.
//
// ================================================================

#pragma once

#include <vector>
#include "abilityData.hpp"
#include "auraData.hpp"
#include "enconterData.hpp"
#include "itemData.hpp"
#include "monsterData.hpp"
#include "raceData.hpp"

class GameData {
public:
	GameData();

	// Loads data from the database into structs.
	void LoadData();

	// Returns ability data for a given ID.
	AbilityData* GetAbility(size_t id);

	// Returns aura data for a given ID.
	AuraData* GetAura(size_t id);

	// Returns encounter data for a given ID.
	EncounterData* GetEncounter(size_t id);

	// Returns item data for a given ID.
	ItemData* GetItem(size_t id);

	// Returns monster data for a given ID.
	MonsterData* GetMonster(size_t id);

	// Returns race data for a given ID.
	RaceData* GetRace(size_t id);

private:
	// Resizes the ability vector to the proper size.
	static int resizeAbilities(void* notUsed, int argc, char** data, char** colname);

	// Initializes the ability data.
	static int loadAbilities(void* notUsed, int argc, char** data, char** colname);
	
	// Initializes the ability stat mods.
	static int loadAbilityStatMods(void* notUsed, int argc, char** data, char** colname);

	// Resizes the aura vector to the proper size.
	static int resizeAuras(void* notUsed, int argc, char** data, char** colname);

	// Initializes the ability data.
	static int loadAuras(void* notUsed, int argc, char** data, char** colname);

	// Initializes the ability stat mods.
	static int loadAuraStatMods(void* notUsed, int argc, char** data, char** colname);

	// Resizes the encounter vector to the proper size.
	static int resizeEncounters(void* notUsed, int argc, char** data, char** colname);

	// Initializes the encounter data.
	static int loadEncounters(void* notUsed, int argc, char** data, char** colname);

	// Resizes the item vector to the proper size.
	static int resizeItems(void* notUsed, int argc, char** data, char** colname);

	// Initializes the item data.
	static int loadItems(void* notUsed, int argc, char** data, char** colname);

	// Initializes the item stat mods.
	static int loadItemStatMods(void* notUsed, int argc, char** data, char** colname);

	// Resizes the monster vector to the proper size.
	static int resizeMonsters(void* notUsed, int argc, char** data, char** colname);

	// Initializes the monster data.
	static int loadMonsters(void* notUsed, int argc, char** data, char** colname);
	
	// Initializes the monster AI data.
	static int loadMonsterAI(void* notUsed, int argc, char** data, char** colname);

	// Initializes the monster stat mods.
	static int loadMonsterStatMods(void* notUsed, int argc, char** data, char** colname);

	// Resizes the race vector to the proper size.
	static int resizeRaces(void* notUsed, int argc, char** data, char** colname);

	// Initializes the race data.
	static int loadRaces(void* notUsed, int argc, char** data, char** colname);

	// Initializes the race stat mods.
	static int loadRaceStatMods(void* notUsed, int argc, char** data, char** colname);

	// ================================

	static std::vector<AbilityData> abilities;
	static std::vector<AuraData> auras;
	static std::vector<EncounterData> encounters;
	static std::vector<ItemData> items;
	static std::vector<MonsterData> monsters;
	static std::vector<RaceData> races;
};

extern GameData gameData;
