// ================================================================
//
// auraData.hpp
// 
// Struct that holds data for an aura.
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

struct AuraData {
	size_t ID = 0;
	std::string Name = "Unknown Aura";
	std::string Icon = "placeholder";
	std::string Description = "Error.";
	std::vector<Category> Categories{};
	std::vector<Element> Elements{};
	int BaseDuration = 0;
	int MaxStacks = 1;
	int TickRate = 0;
	bool IsBuff = false;
	bool IsHidden = false;
	bool IsUnique = false;
	bool IsUniqueByActor = false;
	bool ConsumeOnUse = false;
	bool StacksExpireOneByOne = false;
	bool StacksConsumeOneByOne = false;
	bool MultiplyStatModsByStacks = false;
	bool IsStun = false;
	bool IsDisarm = false;
	bool IsSilence = false;
	bool CanCrit = false;
	std::vector<AbilityEffect> OnTick{};
	std::vector<AbilityEffect> OnExpiry{};
	std::vector<StatMod> StatMods{};
};
