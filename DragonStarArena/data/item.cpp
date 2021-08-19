// ================================================================
//
// item.cpp
//
// ================================================================

#include "item.hpp"

#include "gameData.hpp"
#include "itemData.hpp"
#include "id/equipType.hpp"
#include "id/itemType.hpp"

Item::Item() {

}

Item::Item(size_t itemID) {
	Initialize(itemID);
}

void Item::Initialize(size_t itemID) {
	itemData = gameData.GetItem(itemID);

	if (itemData != nullptr) {
		currentRarity = itemData->ItemRarity;
		currentName = itemData->Name;
	}
}

bool Item::IsNull() {
	return itemData == nullptr;
}

std::string Item::GetName() {
	return currentName;
}

ItemRarity Item::GetItemRarity() {
	return currentRarity;
}

ItemType Item::GetItemType() {
	if (itemData != nullptr) {
		return itemData->ItemType;
	}
	return ItemType::Undefined;
}

EquipType Item::GetEquipType() {
	if (itemData != nullptr) {
		return itemData->EquipType;
	}
	return EquipType::Undefined;
}

bool Item::IsTwoHanded() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		EquipType et = itemData->EquipType;
		if (et == EquipType::Sword2H || et == EquipType::Axe2H || et == EquipType::Mace2H || et == EquipType::Spear || et == EquipType::Bow || et == EquipType::Staff) {
			return true;
		}
	}
	return false;
}

int64_t Item::GetWeaponDamage() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponDamage;
	}
	return 0;
}

int64_t Item::GetWeaponMultiplier() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponMultiplier;
	}
	return 0;
}

int64_t Item::GetWeaponSpeed() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponSpeed;
	}
	return 0;
}

int64_t Item::GetWeaponHitChance() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponHitChance;
	}
	return 0;
}

int64_t Item::GetArmor() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Armor) {
		return itemData->Armor;
	}
	return 0;
}

int64_t Item::GetSpellDefense() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Armor) {
		return itemData->SpellDefense;
	}
	return 0;
}


std::vector<StatMod> Item::GetStatMods() {
	std::vector<StatMod> results{};

	if (itemData != nullptr) {
		results.reserve(itemData->StatMods.size() + randomStatMods.size());
		results.insert(results.end(), itemData->StatMods.begin(), itemData->StatMods.end());
		results.insert(results.end(), randomStatMods.begin(), randomStatMods.end());
	}

	return results;
}