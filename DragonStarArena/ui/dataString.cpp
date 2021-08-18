// ================================================================
//
// dataString.cpp
//
// ================================================================

#include "dataString.hpp"

#include <unordered_map>
#include "../data/statMod.hpp"
#include "../data/id/encounterType.hpp"
#include "../data/id/equipType.hpp"
#include "../data/id/itemRarity.hpp"
#include "../data/id/statModType.hpp"

static std::unordered_map<EncounterType, std::string> encounterTypeMap = {
	{EncounterType::Battle, "Battle"},
	{EncounterType::UniqueBattle, "Boss"},
	{EncounterType::Treasure, "Treasure"},
	{EncounterType::SpecialShop, "Shop"}
};

static std::unordered_map<EquipType, std::string> equipTypeMap = {
	{EquipType::Unarmed, "Unarmed"},
	{EquipType::Sword, "Sword"},
	{EquipType::Belt, "Belt"},
	{EquipType::Ring, "Ring"}
};

static std::unordered_map<ItemRarity, std::string> itemRarityMap = {
	{ItemRarity::Common, ""},
	{ItemRarity::Magical, "#magical "},
	{ItemRarity::Rare, "#rare "},
	{ItemRarity::Epic, "#epic "},
	{ItemRarity::FixedArtifact, "#fixedart "},
	{ItemRarity::RandomArtifact, "#randart "}
};

static std::unordered_map<StatModType, std::string> statModTypeMap = {
	{StatModType::HP, "Maximum HP"},
	{StatModType::MP, "Maximum MP"},
	{StatModType::SP, "Maximum SP"},
	{StatModType::AttackPower, "Attack Power"},
	{StatModType::SpellPower, "Spell Power"},
	{StatModType::HealingPower, "Healing Power"},
	{StatModType::Accuracy, "Accuracy"}
};

std::string DataString::EncounterTypeString(EncounterType encounterType) {
	auto i = encounterTypeMap.find(encounterType);
	if (i != encounterTypeMap.end()) {
		return i->second;
	}
	return "";
}

std::string DataString::EquipTypeString(EquipType equipType) {
	auto i = equipTypeMap.find(equipType);
	if (i != equipTypeMap.end()) {
		return i->second;
	}
	return "";
}

std::string DataString::ItemRarityColorCode(ItemRarity itemRarity) {
	auto i = itemRarityMap.find(itemRarity);
	if (i != itemRarityMap.end()) {
		return i->second;
	}
	return "";
}

std::string DataString::StatModString(StatMod* statMod) {
	std::string str = "";

	if (statMod->Value >= 0) {
		str += "+";
	}

	str += std::to_string(statMod->Value);

	auto i = statModTypeMap.find(statMod->StatModType);
	if (i != statModTypeMap.end()) {
		str += " " + i->second;
	}
	else {
		str += " Unknown";
	}

	return str;
}