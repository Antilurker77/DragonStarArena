// ================================================================
//
// itemData.hpp
// 
// Struct that holds data for an item.
//
// ================================================================

#pragma once

enum class Element;
enum class EquipType;
enum class ItemRarity;
enum class ItemType;

#include <string>
#include <vector>
#include "statMod.hpp"

struct ItemData {
	std::string Name = "Unknown Item";
	ItemRarity ItemRarity{};
	int64_t BaseValue = 0;
	ItemType ItemType{};
	EquipType EquipType{};
	int64_t WeaponDamage = 0;
	int64_t WeaponMultiplier = 0;
	int64_t WeaponSpeed = 0;
	int64_t WeaponHitChance = 0;
	std::vector<Element> WeaponElements{};
	int64_t Armor = 0;
	int64_t SpellDefense = 0;
	std::vector<StatMod> StatMods{};
};
