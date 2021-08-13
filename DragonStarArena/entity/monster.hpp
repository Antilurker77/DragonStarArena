// ================================================================
//
// monster.hpp
//
// Class for the monster characters.
//
// ================================================================

#pragma once

struct MonsterData;

#include "actor.hpp"

class Monster : public Actor {
public:
	Monster();
	Monster(size_t monsterID, BattlePosition p);

	bool IsPlayer();

	int64_t GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs);
private:
	int64_t getBaseStat(StatModType statModType);
	std::vector<StatMod> getBaseStatMods();
	int64_t getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions);

	// ================================

	MonsterData* monsterData = nullptr;
};
