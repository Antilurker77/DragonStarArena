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

	void AwardEXP(int64_t exp);
	bool IsPlayer();

	std::array<Item, 12>* GetEquipment();
	void Equip(Item& item, std::vector<Item>* inventory, size_t index);
	void Equip(Item& item, size_t slot, std::vector<Item>* inventory, size_t index);
	void Unequip(size_t slot, std::vector<Item>* inventory);
	bool IsRanged();
	int GetAttackSpeed();
	int64_t GetMainHandDamage(CombatOptions& combatOptions, bool consumeBuffs);
	int64_t GetEXPDrop();
	int64_t GetGoldDrop();
	int64_t GetLootPoints();
private:
	int64_t getBaseStat(StatModType statModType);
	std::vector<StatMod> getBaseStatMods();
	int64_t getStatFromEquipment(StatModType statModType, CombatOptions& combatOptions);

	// ================================

	MonsterData* monsterData = nullptr;
};
