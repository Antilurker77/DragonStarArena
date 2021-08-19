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

std::string Item::GetIcon() {
	std::string fp = "placeholder";

	if (itemData != nullptr) {
		switch (itemData->ItemType) {
		case ItemType::Weapon:
			switch (itemData->EquipType) {
			case EquipType::Sword:
			case EquipType::Sword2H:
				fp = "sword";
				break;
			case EquipType::Bow:
				fp = "longbow";
				break;
			case EquipType::Staff:
				fp = "staff";
				break;
			default:
				break;
			}
			break;
		case ItemType::Armor:
			switch (itemData->EquipType) {
			case EquipType::Shield:
				fp = "shield";
				break;
			case EquipType::LightHead:
			case EquipType::MediumHead:
			case EquipType::HeavyHead:
				fp = "head";
				break;
			case EquipType::LightBody:
			case EquipType::MediumBody:
			case EquipType::HeavyBody:
				fp = "body";
				break;
			case EquipType::LightHands:
			case EquipType::MediumHands:
			case EquipType::HeavyHands:
				fp = "hands";
				break;
			case EquipType::LightLegs:
			case EquipType::MediumLegs:
			case EquipType::HeavyLegs:
				fp = "legs";
				break;
			case EquipType::LightFeet:
			case EquipType::MediumFeet:
			case EquipType::HeavyFeet:
				fp = "feet";
				break;
			case EquipType::Belt:
				fp = "belt";
				break;
			case EquipType::Neck:
				fp = "neck";
				break;
			case EquipType::Ring:
				fp = "ring";
				break;
			case EquipType::Trinket:
				fp = "trinket";
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	return "gfx/icon/item/" + fp + ".png";
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