// ================================================================
//
// tactic.hpp
// 
// Struct that holds an AI tactic.
//
// ================================================================

#pragma once

#include <cstdint>

enum class AICondition;
enum class AITarget;

struct Tactic {
	size_t Rating = 0;

	AICondition Condition{};
	int64_t ConditionValue = 0;

	AITarget Target{};
	int64_t TargetValue = 0;

	size_t AbilityID = 0;

	int Weight = 100;
};