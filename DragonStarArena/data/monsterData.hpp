// ================================================================
//
// monsterData.hpp
// 
// Struct that holds data for a monster.
//
// ================================================================

#pragma once

enum class EquipType;

#include <string>
#include <vector>
#include "statMod.hpp"
#include "tactic.hpp"

struct MonsterData {
	std::string Name = "Unknown Monster";
	std::string Title = "";
	std::string Graphic = "test_dummy";
	bool IsUnique = false;
	int Level = 1;
	int64_t BaseHP = 100ll;
	int64_t BaseMP = 100ll;
	int64_t BaseSP = 100ll;
	int64_t BaseAttackPower = 10ll;
	int64_t BaseSpellPower = 10ll;
	int64_t BaseHealingPower = 10ll;
	int64_t BaseArmor = 20ll;
	int64_t BaseSpellDefense = 20ll;
	int64_t BaseEvasion = 0ll;
	int64_t BaseHitChance = 8000ll;
	int64_t BaseAccuracy = 1ll;
	int AttackSpeed = 200;
	EquipType MainHandWeaponType{};
	int64_t MainHandWeaponDamage = 0ll;
	int64_t MainHandMultiplier = 10000;
	EquipType OffHandWeaponType{};
	int64_t OffHandWeaponDamage = 0ll;
	int64_t OffHandMultiplier = 10000;
	std::vector<StatMod> StatMods{};
	std::vector<size_t> AbilityIDs{};
	std::vector<Tactic> Tactics{};
	int64_t EXPDrop = 0ll;
	int64_t GoldDrop = 0ll;
	int64_t LootPoints = 0ll;
};