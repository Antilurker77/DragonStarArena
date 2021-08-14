// ================================================================
//
// monster.cpp
//
// ================================================================

#include "monster.hpp"

#include "../data/gameData.hpp"
#include "../data/monsterData.hpp"
#include "../data/id/equipType.hpp"
#include "../data/id/statModType.hpp"

Monster::Monster() {
	name = "Unknown Monster";
	monsterData = nullptr;
	currentHP = 1ll;

	sf::Sprite sprite;
	sprite.setTexture(*assetManager.LoadTexture("gfx/monster/test_dummy.png"));
	sprites.push_back(sprite);
}

Monster::Monster(size_t monsterID, BattlePosition p) {
	monsterData = gameData.GetMonster(monsterID);

	name = "Unknown Monster";
	level = 1;
	battlePosition = p;

	sf::Sprite sprite;
	sprite.setTexture(*assetManager.LoadTexture("gfx/monster/test_dummy.png"));

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();
	currentSP = GetMaxSP();

	abilities.push_back(Ability(1)); // Attack
	if (monsterData != nullptr) {
		name = monsterData->Name;
		sprite.setTexture(*assetManager.LoadTexture("gfx/monster/" + monsterData->Graphic + ".png"));
		level = monsterData->Level;
		
		for (size_t i = 0; i < monsterData->AbilityIDs.size(); i++) {
			abilities.push_back(Ability(monsterData->AbilityIDs[i]));
		}

		tactics = monsterData->Tactics;
	}

	sprites.push_back(sprite);
}

bool Monster::IsPlayer() {
	return false;
}

int64_t Monster::GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs) {
	int64_t result = 0;
	if (monsterData != nullptr) {
		if (monsterData->MainHandWeaponType == EquipType::Unarmed) {
			result = static_cast<int64_t>(std::round(5.0 + (level - 1.0) * 0.5 + (std::pow(level - 1.0, 2) * 0.05)));
		}
		else {
			result = monsterData->MainHandWeaponDamage;
		}
		result += GetAttackPower(combatOptions, consumeBuffs) * monsterData->MainHandMultiplier / 10000;
	}
	return result;
}

int64_t Monster::getBaseStat(StatModType statModType) {
	int64_t result = 0;

	if (monsterData != nullptr) {
		switch (statModType) {
		case StatModType::HP:
			result = monsterData->BaseHP;
			break;
		case StatModType::MP:
			result = monsterData->BaseMP;
			break;
		case StatModType::SP:
			result = monsterData->BaseSP;
			break;
		case StatModType::AttackPower:
			result = monsterData->BaseAttackPower;
			break;
		case StatModType::SpellPower:
			result = monsterData->BaseSpellPower;
			break;
		case StatModType::HealingPower:
			result = monsterData->BaseHealingPower;
			break;
		case StatModType::Armor:
			result = monsterData->BaseArmor;
			break;
		case StatModType::SpellDefense:
			result = monsterData->BaseSpellDefense;
			break;
		default:
			break;
		}
	}
	return result;
}

std::vector<StatMod> Monster::getBaseStatMods() {
	if (monsterData != nullptr) {
		return monsterData->StatMods;
	}
	return {};
}

int64_t Monster::getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions) {
	return 0ll;
}