// ================================================================
//
// combat.hpp
// 
// Set of functions for handling combat actions and their results.
//
// ================================================================

#pragma once

class Actor;
class BattleScene;
struct AbilityData;
enum class Attribute;
enum class Category;
enum class Element;

#include <string>
#include <vector>

struct CombatOptions {
	std::string EventName = "Unknown Event";

	std::vector<Category> Categories{};
	std::vector<Element> Elements{};

	size_t AbilityID = 0;
	size_t AuraID = 0;

	int BaseHitChance = 8000;

	bool CanDodge = false;
	bool CanBlock = false;
	bool CanCounter = false;
	bool CanCrit = false;
	bool CanDoubleStrike = false;
};

struct CombatResult {
	int64_t Amount = 0;

	bool DidHit = false;
	bool DidCrit = false;
	bool DidDoubleStrike = false;
	bool DidKill = false;
	bool DidBlock = false;
};

namespace Combat {
	// Deals damage based on the user's weapon.
	// 1 Coefficient = 0.01% Multiplier
	CombatResult WeaponDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene);

	// Deals damage based on the user's attack power.
	// Base damage determined by level and coefficient. 1 Coefficient = 0.01% Multiplier
	CombatResult AttackDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene);

	// Deals damage based on the user's spell power.
	// Base damage determined by level and coefficient. 1 Coefficient = 0.01% Multiplier
	CombatResult SpellDamage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene);

	// Heals based on the user's healing power.
	// Base healing determined by level and coefficient. 1 Coefficient = 0.01% Multiplier
	CombatResult Healing(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t coefficient, Attribute attribute, BattleScene* battleScene);

	// Heals based on the target's maximum attribute.
	// 1 Percent = 0.01% Multiplier
	CombatResult HealingPercent(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t percent, Attribute attribute, BattleScene* battleScene);

	// Adds the aura to the target.
	void AddAura(Actor* user, Actor* target, size_t auraID, int stacks, BattleScene* battleScene);
}