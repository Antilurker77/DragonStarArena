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
enum class ItemRarity;

#include <string>
#include <vector>

namespace DataString {
	// Returns the color code based on the given item rarity.
	std::string ItemRarityColorCode(ItemRarity itemRarity);

	// Returns a string based on the given stat mod.
	std::string StatModString(StatMod* statMod);
}