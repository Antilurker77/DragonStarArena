// ================================================================
//
// abilityData.hpp
// 
// Struct that holds data for an ability.
//
// ================================================================

#pragma once

enum class Category;
enum class Element;
enum class EquipType;

#include <string>
#include <vector>
#include "abilityEffect.hpp"
#include "statMod.hpp"

enum class TargetType {
	Undefined = 0,
	User,
	Ally,
	Enemy
};

enum class TargetScope {
	Undefined = 0,
	Single,
	Row
};

struct AbilityData {
	size_t ID = 0;
	std::string Name = "Unknown Ability";
	std::string Icon = "placeholder.png";
	std::string Description = "Error.";
	bool IsPassive = false;
	std::vector<Category> Categories{};
	std::vector<Element> Elements{};
	std::vector<EquipType> RequiredWeaponTypes{};
	TargetType TargetType{};
	TargetScope TargetScope{};
	bool IsRanged = false;
	int UseTime = 100;
	int Cooldown = 0;
	int MaxCharges = 1;
	int64_t MPCost = 0;
	int64_t SPCost = 0;
	int BaseHitChance = 8000;
	bool CanDodge = false;
	bool CanBlock = false;
	bool CanCrit = false;
	bool CanDoubleStrike = false;
	bool OffHandAttack = false;
	std::vector<AbilityEffect> Effects{};
	std::vector<StatMod> StatMods{};
};