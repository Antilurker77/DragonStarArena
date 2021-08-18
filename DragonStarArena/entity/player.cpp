// ================================================================
//
// player.cpp
//
// ================================================================

#include "player.hpp"

#include "../data/gameData.hpp"
#include "../data/id/aiCondition.hpp"
#include "../data/id/aiTarget.hpp"
#include "../data/id/statModType.hpp"

Player::Player() {
	name = "Unnamed Player";
	level = 1;
	raceData = nullptr;

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();
	currentSP = GetMaxSP();
}

Player::Player(std::string name, size_t raceID, BattlePosition p) {
	level = 1;
	battlePosition = p;

	this->name = name;
	raceData = gameData.GetRace(raceID);

	if (IsInFrontRow()) {
		abilities.push_back(Ability(1)); // Attack
		abilities.push_back(Ability(4)); // Power Strike

		tactics.push_back({ 0, AICondition::None, 0, AITarget::Any, 0, 4, 100 });
		tactics.push_back({ 1, AICondition::None, 0, AITarget::Any, 0, 1, 100 });

		equipment[0].Initialize(5);
		equipment[3].Initialize(8);
		equipment[5].Initialize(9);
		equipment[6].Initialize(10);
	}
	else {
		abilities.push_back(Ability(2)); // Shoot
		abilities.push_back(Ability(10)); // Power Shot

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

int Player::GetAttackSpeed() {
	int result = 200;

	if (!equipment[0].IsNull()) {
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