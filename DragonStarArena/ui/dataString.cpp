// ================================================================
//
// dataString.cpp
//
// ================================================================

#include "dataString.hpp"

#include <sstream>
#include <unordered_map>
#include "../core/combat.hpp"
#include "../data/ability.hpp"
#include "../data/statMod.hpp"
#include "../data/id/attribute.hpp"
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
	{EquipType::MediumBody, "Medium Body Armor"},
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

std::vector<std::string> DataString::FormatAbilityDescription(Ability* ability, Actor* user, size_t charsPerLine) {
	std::vector<std::string> strv{};
	std::stringstream ss(ability->GetDescription());
	CombatOptions co = ability->GetCombatOptions();
	std::vector<std::string> words{};
	std::string temp;
	std::string currentLine;
	size_t charCount = 0;

	while (std::getline(ss, temp, ' ')) {
		words.push_back(temp);
	}

	for (size_t i = 0; i < words.size(); i++) {
		std::vector<std::string> wordsToAdd{};
		if (words[i][0] == '/') {
			int64_t coefficient = std::stoll(words[i].substr(2));
			// weapon damge
			if (words[i][1] == 'w') {
				wordsToAdd.push_back("#damage");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::WeaponDamageEstimate(user, co, coefficient)));
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
				}
				wordsToAdd.push_back("#default");
			}
			// attack damage
			else if (words[i][1] == 'a') {
				wordsToAdd.push_back("#damage");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::AttackDamageEstimate(user, co, coefficient)));
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
					wordsToAdd.push_back("Attack");
					wordsToAdd.push_back("Power");
				}
				wordsToAdd.push_back("#default");
			}
			// spell damage
			else if (words[i][1] == 's') {
				wordsToAdd.push_back("#damage");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::SpellDamageEstimate(user, co, coefficient)));
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
					wordsToAdd.push_back("Spell");
					wordsToAdd.push_back("Power");
				}
				wordsToAdd.push_back("#default");
			}
			// healing
			else if (words[i][1] == 'h') {
				wordsToAdd.push_back("#heal");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::HealingEstimate(user, co, coefficient)));
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
					wordsToAdd.push_back("Healing");
					wordsToAdd.push_back("Power");
				}
				wordsToAdd.push_back("#default");
			}
			// healing hp percent
			else if (words[i][1] == 'e') {
				wordsToAdd.push_back("#heal");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::HealingPercentSelfEstimate(user, co, coefficient, Attribute::HP)));
					wordsToAdd.push_back("(" + std::to_string(coefficient / 100) + "%)");
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
				}
				wordsToAdd.push_back("#default");
			}
			// healing mp percent
			else if (words[i][1] == 'm') {
				wordsToAdd.push_back("#spell");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::HealingPercentSelfEstimate(user, co, coefficient, Attribute::MP)));
					wordsToAdd.push_back("(" + std::to_string(coefficient / 100) + "%)");
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
				}
				wordsToAdd.push_back("#default");
			}
			// healing sp percent
			else if (words[i][1] == 't') {
				wordsToAdd.push_back("#skill");
				if (user != nullptr) {
					wordsToAdd.push_back(std::to_string(Combat::HealingPercentSelfEstimate(user, co, coefficient, Attribute::SP)));
					wordsToAdd.push_back("(" + std::to_string(coefficient / 100) + "%)");
				}
				else {
					wordsToAdd.push_back(std::to_string(coefficient / 100) + "%");
				}
				wordsToAdd.push_back("#default");
			}
		}
		// normal words
		else {
			wordsToAdd.push_back(words[i]);
		}

		for (size_t j = 0; j < wordsToAdd.size(); j++) {
			if (wordsToAdd[j][0] == '#') {
				currentLine += " " + wordsToAdd[j];
			}
			else {
				size_t wordLength = wordsToAdd[j].length();
				if (charCount + wordLength + 1 > charsPerLine) {
					strv.push_back(currentLine);
					if (j > 0) {
						currentLine = wordsToAdd[0] + wordsToAdd[j];
					}
					else {
						currentLine = wordsToAdd[j];
					}
					charCount = wordLength;
				}
				else {
					if (charCount != 0) {
						currentLine += " ";
					}
					currentLine += wordsToAdd[j];
					charCount += wordLength + 1;
				}
			}
		}
	}

	strv.push_back(currentLine);

	return strv;
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

std::string DataString::TimeString(int64_t time) {
	std::string str = "";

	str = std::to_string(time / 100) + ".";

	int64_t remainder = time % 100;
	if (remainder < 10) {
		str += "0";
	}
	str += std::to_string(remainder) + "s";

	return str;
}