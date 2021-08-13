// ================================================================
//
// encounterData.hpp
// 
// Struct that holds data for an enemy encounter.
//
// ================================================================

#pragma once

#include <string>

struct EncounterData {
	std::string Name = "Unknown Encounter";
	bool IsUnique = false;
	int MinLevel = 1;
	int MaxLevel = 255;
	int Weight = 10000;
	size_t FrontFarLeftMonster = 0;
	size_t FrontLeftMonster = 0;
	size_t FrontCenterMonster = 0;
	size_t FrontRightMonster = 0;
	size_t FrontFarRightMonster = 0;
	size_t BackFarLeftMonster = 0;
	size_t BackLeftMonster = 0;
	size_t BackCenterMonster = 0;
	size_t BackRightMonster = 0;
	size_t BackFarRightMonster = 0;
};