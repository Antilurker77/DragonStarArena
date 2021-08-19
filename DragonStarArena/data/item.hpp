// ================================================================
//
// item.hpp
// 
// Class for an individual instance of an item.
//
// ================================================================

#pragma once

enum class EquipType;
enum class ItemRarity;
enum class ItemType;
struct ItemData;

#include <string>
#include <vector>
#include "statMod.hpp"

class Item {
public:
	Item();
	Item(size_t itemID);

	// Initializes the item based on the given ID.
	void Initialize(size_t itemID);

	// Returns true if the item is uninitialized.
	bool IsNull();

	// Returns the item's name.
	std::string GetName();

	// Returns the filepath of the icon.
	std::string GetIcon();

	// Return the item's rarity.
	ItemRarity GetItemRarity();

	// Returns the item's type.
	ItemType GetItemType();

	// Returns the equip type of the equipment.
	EquipType GetEquipType();

	// Returns true if the weapon is two handed.
	bool IsTwoHanded();

	// Returns the damage dealt by the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponDamage();

	// Returns the attack power multiplier of the weapon.
	// Non-weapons return 0.
	int64_t GetWeaponMultiplier();

	// Returns the attack speed of the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponSpeed();

	// Returns the base hit chance of the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponHitChance();

	// Returns the armor value of the armor. Non-armor returns 0.
	int64_t GetArmor();

	// Returns the spell defense value of the armor. Non-armor returns 0.
	int64_t GetSpellDefense();

	// Returns the stat mods of the item.
	std::vector<StatMod> GetStatMods();

private:
	ItemData* itemData = nullptr;
	std::string currentName = "Unnamed Item";
	ItemRarity currentRarity{};
	std::vector<StatMod> randomStatMods;
};