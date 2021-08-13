// ================================================================
//
// abilityEffect.hpp
// 
// Struct that holds a single effect an ability can execute.
//
// ================================================================

#pragma once

#include <vector>
#include "id/attribute.hpp"

enum class Effect {
	Undefined = 0,

	// 0: Coefficient
	WeaponDamage,

	// 0: Coefficient, 1: AuraID, 2: Stack Count, 3: Application Chance
	WeaponDamageAuraHit,

	// 0: Coefficient
	AttackDamage,

	// 0: Coefficient
	SpellDamage,

	// 0: Coefficient, 1: AuraID, 2: Stack Count, 3: Application Chance
	SpellDamageAuraHit,

	// 0: Coefficient
	Healing,

	// 0: Percent
	HealingPercent
};

struct AbilityEffect {
	Effect Effect{};
	std::vector<int64_t> Arguments{};
	Attribute Attribute{};
};
