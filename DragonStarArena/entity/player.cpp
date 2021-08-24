// ================================================================
//
// player.cpp
//
// ================================================================

#include "player.hpp"

#include "../data/gameData.hpp"
#include "../data/id/aiCondition.hpp"
#include "../data/id/aiTarget.hpp"
#include "../data/id/equipType.hpp"
#include "../data/id/itemType.hpp"
#include "../data/id/statModType.hpp"

Player::Player() {
	name = "Unnamed Player";
	level = 1;
	raceData = nullptr;

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();
	currentSP = GetMaxSP();

	abilities.resize(11);
	abilities[0].Initialize(1);
}

Player::Player(std::string name, size_t raceID, BattlePosition p) {
	level = 1;
	battlePosition = p;

	this->name = name;
	raceData = gameData.GetRace(raceID);

	abilities.resize(11);

	abilityPool.push_back(3); // Magic Missile
	abilityPool.push_back(6); // Constitution
	abilityPool.push_back(7); // Second Wind
	abilityPool.push_back(8); // Heal
	abilityPool.push_back(12); // Flame Bolt

	if (IsInFrontRow()) {
		abilities[0].Initialize(1); // Attack
		abilities[1].Initialize(4); // Power Strike

		tactics.push_back({ 0, AICondition::None, 0, AITarget::Any, 0, 4, 100 });
		tactics.push_back({ 1, AICondition::None, 0, AITarget::Any, 0, 1, 100 });

		equipment[0].Initialize(5);
		equipment[3].Initialize(8);
		equipment[5].Initialize(9);
		equipment[6].Initialize(10);
	}
	else {
		abilities[0].Initialize(2); // Shoot
		abilities[1].Initialize(10); // Power Shot

		tactics.push_back({ 0, AICondition::None, 0, AITarget::Any, 0, 10, 100 });
		tactics.push_back({ 1, AICondition::None, 0, AITarget::Any, 0, 2, 100 });

		equipment[0].Initialize(6);
		equipment[3].Initialize(8);
		equipment[5].Initialize(9);
		equipment[6].Initialize(10);
	}

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();
	currentSP = GetMaxSP();
}

void Player::AwardEXP(int64_t exp) {
	double baseEXP = 100.0;
	if (raceData != nullptr) {
		baseEXP = static_cast<double>(raceData->BaseEXPPerLevel);
	}
	int64_t nextLevel = static_cast<int64_t>(std::round(baseEXP + (level - 1.0) * (baseEXP / 10.0) + std::pow(level - 1.0, 3.0) * (baseEXP / 100.0)));
	currentEXP += exp * GetEXPRate() / 10000ll;
	while (currentEXP >= nextLevel) {
		currentEXP -= nextLevel;
		level++;
		nextLevel = static_cast<int64_t>(std::round(baseEXP + (level - 1.0) * (baseEXP / 10.0) + std::pow(level - 1.0, 3.0) * (baseEXP / 100.0)));
	}
}

bool Player::IsPlayer() {
	return true;
}

std::array<Item, 12>* Player::GetEquipment() {
	return &equipment;
}

void Player::Equip(Item& item, std::vector<Item>* inventory, size_t index) {
	size_t slot = 0;

	switch (item.GetEquipType()) {
	case EquipType::Sword:
	case EquipType::Axe:
	case EquipType::Mace:
	case EquipType::Dagger:
		if (equipment[0].IsNull() || equipment[0].IsTwoHanded()) {
			slot = 0;
		}
		else if (equipment[0].GetEquipType() == EquipType::Wand) {
			slot = 0;
		}
		else if (equipment[1].GetEquipType() == EquipType::Shield || equipment[1].GetEquipType() == EquipType::OffHand) {
			slot = 0;
		}
		else if (!equipment[0].IsNull() && !equipment[0].IsTwoHanded()) {
			slot = 1;
		}
		else {
			slot = 0;
		}
		break;
	case EquipType::Sword2H:
	case EquipType::Axe2H:
	case EquipType::Mace2H:
	case EquipType::Spear:
	case EquipType::Bow:
	case EquipType::Wand:
	case EquipType::Staff:
		slot = 0;
		break;
	case EquipType::Shield:
	case EquipType::OffHand:
		slot = 1;
		break;
	case EquipType::LightHead:
	case EquipType::MediumHead:
	case EquipType::HeavyHead:
		slot = 2;
		break;
	case EquipType::LightBody:
	case EquipType::MediumBody:
	case EquipType::HeavyBody:
		slot = 3;
		break;
	case EquipType::LightHands:
	case EquipType::MediumHands:
	case EquipType::HeavyHands:
		slot = 4;
		break;
	case EquipType::LightLegs:
	case EquipType::MediumLegs:
	case EquipType::HeavyLegs:
		slot = 5;
		break;
	case EquipType::LightFeet:
	case EquipType::MediumFeet:
	case EquipType::HeavyFeet:
		slot = 6;
		break;
	case EquipType::Belt:
		slot = 7;
		break;
	case EquipType::Neck:
		slot = 8;
		break;
	case EquipType::Ring:
		if (equipment[9].IsNull()) {
			slot = 9;
		}
		else if (equipment[10].IsNull()) {
			slot = 10;
		}
		else {
			slot = 9;
		}
		break;
	case EquipType::Trinket:
		slot = 11;
		break;
	default:
		return;
	}

	Equip(item, slot, inventory, index);
}

void Player::Equip(Item& item, size_t slot, std::vector<Item>* inventory, size_t index) {
	static std::vector<std::vector<EquipType>> allowedTypes{
		{EquipType::Sword, EquipType::Sword2H, EquipType::Axe, EquipType::Axe2H, EquipType::Mace, EquipType::Mace2H, EquipType::Dagger, EquipType::Spear, EquipType::Bow, EquipType::Wand, EquipType::Staff},
		{EquipType::Sword, EquipType::Axe, EquipType::Mace, EquipType::Dagger, EquipType::Shield, EquipType::OffHand},
		{EquipType::LightHead, EquipType::MediumHead, EquipType::HeavyHead},
		{EquipType::LightBody, EquipType::MediumBody, EquipType::HeavyBody},
		{EquipType::LightHands, EquipType::MediumHands, EquipType::HeavyHands},
		{EquipType::LightLegs, EquipType::MediumLegs, EquipType::HeavyLegs},
		{EquipType::LightFeet, EquipType::MediumFeet, EquipType::HeavyFeet},
		{EquipType::Belt},
		{EquipType::Neck},
		{EquipType::Ring},
		{EquipType::Ring},
		{EquipType::Trinket}
	};

	int64_t missingHP = GetMaxHP() - currentHP;
	int64_t missingMP = GetMaxMP() - currentMP;
	int64_t missingSP = GetMaxSP() - currentSP;

	if (std::find(allowedTypes[slot].begin(), allowedTypes[slot].end(), item.GetEquipType()) != allowedTypes[slot].end()) {
		if (slot == 0) {
			if (!equipment[0].IsNull()) {
				inventory->push_back(equipment[0]);
			}
			equipment[0] = item;
			if (item.IsTwoHanded()) {
				if (!equipment[1].IsNull()) {
					inventory->push_back(equipment[1]);
					equipment[1] = Item();
				}
			}

			if (IsRanged()) {
				abilities[0].Initialize(2);
			}
			else {
				abilities[0].Initialize(1);
			}
		}
		else if (slot == 1) {
			if (!equipment[1].IsNull()) {
				inventory->push_back(equipment[1]);
			}
			equipment[1] = item;
			if (equipment[0].IsTwoHanded()) {
				inventory->push_back(equipment[0]);
				equipment[0] = Item();
			}
			abilities[0].Initialize(1);
		}
		else {
			if (!equipment[slot].IsNull()) {
				inventory->push_back(equipment[slot]);
			}
			equipment[slot] = item;
		}

		inventory->erase(inventory->begin() + index);
	}

	if (currentHP > 0) {
		currentHP = GetMaxHP() - missingHP;
		currentMP = GetMaxMP() - missingMP;
		currentSP = GetMaxSP() - missingSP;
	}
}

void Player::Unequip(size_t slot, std::vector<Item>* inventory) {
	int64_t missingHP = GetMaxHP() - currentHP;
	int64_t missingMP = GetMaxMP() - currentMP;
	int64_t missingSP = GetMaxSP() - currentSP;

	if (!equipment[slot].IsNull()) {
		inventory->push_back(equipment[slot]);
		equipment[slot] = Item();
	}

	if (slot == 0) {
		abilities[0].Initialize(1);
	}

	if (currentHP > 0) {
		currentHP = GetMaxHP() - missingHP;
		currentMP = GetMaxMP() - missingMP;
		currentSP = GetMaxSP() - missingSP;
	}
}

void Player::EquipAbility(size_t abilityID) {
	size_t slot = 0;

	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i].GetAbilityID() == 0) {
			slot = i;
			break;
		}
	}

	if (slot != 0) {
		EquipAbility(abilityID, slot);
	}
}

void Player::EquipAbility(size_t abilityID, size_t slot) {
	int64_t missingHP = GetMaxHP() - currentHP;
	int64_t missingMP = GetMaxMP() - currentMP;
	int64_t missingSP = GetMaxSP() - currentSP;
	
	if (abilities[slot].GetAbilityID() != 0) {
		abilityPool.push_back(abilities[slot].GetAbilityID());
	}
	abilities[slot].Initialize(abilityID);
	auto i = std::find(abilityPool.begin(), abilityPool.end(), abilityID);
	abilityPool.erase(i);

	if (currentHP > 0) {
		currentHP = GetMaxHP() - missingHP;
		currentMP = GetMaxMP() - missingMP;
		currentSP = GetMaxSP() - missingSP;
	}
}

void Player::UnequipAbility(size_t slot) {
	if (abilities[slot].GetAbilityID() != 0) {
		abilityPool.push_back(abilities[slot].GetAbilityID());
		abilities[slot] = Ability();
	}
}

bool Player::IsRanged() {
	if (!equipment[0].IsNull()) {
		EquipType weaponType = equipment[0].GetEquipType();
		if (weaponType == EquipType::Bow || weaponType == EquipType::Wand) {
			return true;
		}
	}
	return false;
}

int Player::GetAttackSpeed() {
	int result = 200;

	if (!equipment[0].IsNull()) {
		if (!equipment[1].IsNull() && equipment[1].GetItemType() == ItemType::Weapon) {
			result = static_cast<int>(std::max(equipment[0].GetWeaponSpeed(), equipment[1].GetWeaponSpeed()));
		}
		result = static_cast<int>(equipment[0].GetWeaponSpeed());
	}

	return result;
}

int64_t Player::GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0;

	if (!equipment[0].IsNull()) {
		result = equipment[0].GetWeaponDamage();
		result += GetAttackPower(combatOptions, consumeBuffs * equipment[0].GetWeaponMultiplier() / 10000);
	}
	else {
		result = static_cast<int64_t>(std::round(5.0 + (level - 1.0) * 0.5 + (std::pow(level - 1.0, 2) * 0.05)));
		result += GetAttackPower(combatOptions, consumeBuffs);
	}

	return result;
}

int64_t Player::GetEXPDrop() {
	return 0ll;
}

int64_t Player::GetGoldDrop() {
	return 0ll;
}

int64_t Player::GetLootPoints() {
	return 0ll;
}

int64_t Player::getBaseStat(StatModType statModType) {
	int64_t result = 0;

	if (raceData != nullptr) {
		switch (statModType) {
		case StatModType::HP:
			result = raceData->BaseHP;
			result = static_cast<int64_t>(std::round(result + (level - 1.0) * (result / 10.0) + std::pow(level - 1.0, 3.0) * (result / 100.0)));
			break;
		case StatModType::MP:
			result = raceData->BaseMP;
			result = static_cast<int64_t>(std::round(result + (level - 1.0) * (result / 10.0)));
			break;
		case StatModType::SP:
			result = raceData->BaseSP;
			break;
		case StatModType::AttackPower:
			result = 0;
			break;
		case StatModType::SpellPower:
			result = 0;
			break;
		case StatModType::HealingPower:
			result = 0;
			break;
		case StatModType::Accuracy:
			result = 1;
			break;
		case StatModType::Evasion:
			result = 0;
			break;
		default:
			break;
		}
	}
	return result;
}

std::vector<StatMod> Player::getBaseStatMods() {
	if (raceData != nullptr) {
		return raceData->StatMods;
	}
	return {};
}

int64_t Player::getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions) {
	int64_t result = 0;
	std::vector<StatMod> statMods{};

	// element vector compaison lambda, just checking if there are any matches
	auto elementCheck = [](std::vector<Element>& v1, std::vector<Element>& v2) {
		for (auto e : v2) {
			if (std::find(v1.begin(), v1.end(), e) != v1.end()) {
				return true;
			}
		}
		return false;
	};

	// stat mod check lambda
	auto statModCheck = [&](StatMod& s) {
		if (s.StatModType == statModType) {
			bool match = false;
			if (combatOptions.AbilityID != 0 && combatOptions.AbilityID == s.AbilityID) {
				match = true;
			}
			else if (combatOptions.AuraID != 0 && combatOptions.AuraID == s.AuraID) {
				match = true;
			}
			else if (s.Elements.empty() || elementCheck(combatOptions.Elements, s.Elements)) {
				// must be sorted for std::includes to work	
				std::sort(s.Categories.begin(), s.Categories.end());
				if (s.Categories.empty() || std::includes(combatOptions.Categories.begin(), combatOptions.Categories.end(), s.Categories.begin(), s.Categories.end())) {
					match = true;
				}
			}
			if (match) {
				int64_t value = s.Value;

				// resistances only partially apply to multi-element attacks
				if (s.StatModType == StatModType::Resistance) {
					result += value / static_cast<int64_t>(combatOptions.Elements.size());
				}
				else {
					result += value;
				}
			}
		}
	};

	for (size_t i = 0; i < equipment.size(); i++) {
		if (!equipment[i].IsNull()) {
			if (statModType == StatModType::Armor) {
				result += equipment[i].GetArmor();
			}
			else if (statModType == StatModType::SpellDefense) {
				result += equipment[i].GetSpellDefense();
			}

			statMods = equipment[i].GetStatMods();
			for (auto& sm : statMods) {
				statModCheck(sm);
			}
		}
	}

	return result;
}