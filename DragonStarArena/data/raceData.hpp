// ================================================================
//
// raceData.hpp
// 
// Struct that holds data for a player race.
//
// ================================================================

#pragma once

#include <string>
#include <vector>
#include "statMod.hpp"

struct RaceData {
	std::string Name = "Unknown Race";
	int64_t BaseHP = 100;
	int64_t BaseMP = 100;
	int64_t BaseSP = 100;
	int64_t BaseEXPPerLevel = 100;
	std::vector<StatMod> StatMods{};
};