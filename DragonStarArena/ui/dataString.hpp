// ================================================================
//
// dataString.hpp
// 
// List of functions for turning data elements into strings.
//
// ================================================================

#pragma once

struct StatMod;
enum class Category;
enum class Element;
enum class EncounterType;
enum class EquipType;
enum class ItemRarity;

#include <string>
#include <vector>

namespace DataString {
	// Returns the encounter type string.
	std::string EncounterTypeString(EncounterType encounterType);
	
	// Returns the equip type string.
	std::string EquipTypeString(EquipType equipType);
	
	// Returns the color code based on the given item rarity.
	std::string ItemRarityColorCode(ItemRarity itemRarity);

	// Returns a string based on the given stat mod.
	std::string StatModString(StatMod* statMod);

	// Converts a number to a time amount.
	std::string TimeString(int64_t time);
}