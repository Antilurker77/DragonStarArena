// ================================================================
//
// statMod.hpp
// 
// Struct that modifies a stat.
//
// ================================================================

#pragma once

enum class Category;
enum class Element;
enum class StatModType;

#include <vector>

struct StatMod {
	StatModType StatModType{};
	int64_t Value = 0;
	std::vector<Category> Categories{};
	std::vector<Element> Elements{};
	size_t AbilityID = 0;
	size_t AuraID = 0;
};