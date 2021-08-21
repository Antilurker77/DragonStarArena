// ================================================================
//
// gameData.cpp
//
// ================================================================

#include "gameData.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "abilityEffect.hpp"
#include "statMod.hpp"
#include "tactic.hpp"
#include "id/aiCondition.hpp"
#include "id/aiTarget.hpp"
#include "id/category.hpp"
#include "id/element.hpp"
#include "id/encounterType.hpp"
#include "id/equipType.hpp"
#include "id/itemRarity.hpp"
#include "id/itemType.hpp"
#include "id/statModType.hpp"
#include "../core/random.hpp"

GameData gameData;

std::vector<AbilityData> GameData::abilities{};
std::vector<AuraData> GameData::auras{};
std::vector<EncounterData> GameData::encounters{};
std::vector<std::pair<EncounterType, int>> GameData::encounterTypeWeights{};
std::vector<ItemData> GameData::items{};
std::vector<MonsterData> GameData::monsters{};
std::vector<RaceData> GameData::races{};

static std::vector<std::string> splitString(const char* s, char splitter) {
	std::string str;
	std::string temp;
	std::stringstream ss;
	std::vector<std::string> strv{};

	ss.str(s);
	while (std::getline(ss, temp, ' ')) {
		str += temp;
	}

	ss.clear();
	ss.str(str);
	while (std::getline(ss, temp, splitter)) {
		strv.push_back(temp);
	}

	return strv;
}

static std::vector<size_t> stringToID(std::vector<std::string>& strv) {
	std::vector<size_t> ids;

	for (size_t i = 0; i < strv.size(); i++) {
		ids.push_back(std::stoll(strv[i]));
	}

	return ids;
}

static AbilityEffect stringToAbilityEffect(std::string str) {
	AbilityEffect effect;
	std::vector<std::string> strv = splitString(str.c_str(), ',');

	if (strv[0] == "wpn") {
		effect.Effect = Effect::WeaponDamage;
	}
	else if (strv[0] == "wpn_ah") {
		effect.Effect = Effect::WeaponDamageAuraHit;
	}
	else if (strv[0] == "atk") {
		effect.Effect = Effect::AttackDamage;
	}
	else if (strv[0] == "spl") {
		effect.Effect = Effect::SpellDamage;
	}
	else if (strv[0] == "spl_ah") {
		effect.Effect = Effect::SpellDamageAuraHit;
	}
	else if (strv[0] == "heal") {
		effect.Effect = Effect::Healing;
	}
	else if (strv[0] == "heal_p") {
		effect.Effect = Effect::HealingPercent;
	}

	if (strv[1] == "hp") {
		effect.Attribute = Attribute::HP;
	}
	else if (strv[1] == "mp") {
		effect.Attribute = Attribute::MP;
	}
	else if (strv[1] == "sp") {
		effect.Attribute = Attribute::SP;
	}

	for (size_t i = 2; i < strv.size(); i++) {
		effect.Arguments.push_back(std::stoll(strv[i]));
	}

	return effect;
}

static AICondition stringToAICondition(char* s) {
	std::string str(s);

	if (str == "none") {
		return AICondition::None;
	}
	else if (str == "hp_less_than") {
		return AICondition::HPPercentLessThan;
	}
	else if (str == "hp_greater_than") {
		return AICondition::HPPercentGreaterThan;
	}
	else {
		std::cout << "AI Condition Error: Could not parse " << str << ".\n";
		return AICondition::Undefined;
	}
}

static AITarget stringToAITarget(char* s) {
	std::string str(s);

	if (str == "any") {
		return AITarget::Any;
	}
	else if (str == "hp_less_than") {
		return AITarget::HPPercentLessThan;
	}
	else if (str == "hp_greater_than") {
		return AITarget::HPPercentGreaterThan;
	}
	else {
		std::cout << "AI Target Error: Could not parse " << str << ".\n";
		return AITarget::Undefined;
	}
}

static std::vector<Category> stringToCategory(std::vector<std::string>& strv) {
	std::vector<Category> categories;

	for (size_t i = 0; i < strv.size(); i++) {
		if (strv[i] == "damaging") {
			categories.push_back(Category::Damaging);
		}
		else if (strv[i] == "healing") {
			categories.push_back(Category::Healing);
		}
		else if (strv[i] == "single_target") {
			categories.push_back(Category::SingleTarget);
		}
		else if (strv[i] == "area_of_effect") {
			categories.push_back(Category::AreaOfEffect);
		}
		else if (strv[i] == "direct") {
			categories.push_back(Category::Direct);
		}
		else if (strv[i] == "over_time") {
			categories.push_back(Category::OverTime);
		}
		else if (strv[i] == "attack") {
			categories.push_back(Category::Attack);
		}
		else if (strv[i] == "skill") {
			categories.push_back(Category::Skill);
		}
		else if (strv[i] == "tech") {
			categories.push_back(Category::Tech);
		}
		else if (strv[i] == "spell") {
			categories.push_back(Category::Spell);
		}
		else if (strv[i] == "passive") {
			categories.push_back(Category::Passive);
		}
		else {
			std::cout << "Category Error: Could not parse " << strv[i] << ".\n";
		}
	}

	return categories;
}

static std::vector<Element> stringToElements(std::vector<std::string>& strv) {
	std::vector<Element> elements{};

	for (size_t i = 0; i < strv.size(); i++) {
		if (strv[i] == "none") {
			elements.push_back(Element::None);
		}
		else if (strv[i] == "physical") {
			elements.push_back(Element::Physical);
		}
		else if (strv[i] == "arcane") {
			elements.push_back(Element::Arcane);
		}
		else if (strv[i] == "fire") {
			elements.push_back(Element::Fire);
		}
		else if (strv[i] == "ice") {
			elements.push_back(Element::Ice);
		}
		else if (strv[i] == "lightning") {
			elements.push_back(Element::Lightning);
		}
		else if (strv[i] == "water") {
			elements.push_back(Element::Water);
		}
		else if (strv[i] == "poison") {
			elements.push_back(Element::Poison);
		}
		else if (strv[i] == "light") {
			elements.push_back(Element::Light);
		}
		else if (strv[i] == "dark") {
			elements.push_back(Element::Dark);
		}
		else if (strv[i] == "astral") {
			elements.push_back(Element::Astral);
		}
		//else if (strv[i] == "healing") {
		//	elements.push_back(Element::Healing);
		//}
		else {
			std::cout << "Element Error: Could not parse " << strv[i] << ".\n";
		}
	}

	return elements;
}

static EncounterType stringToEncounterType(char* s) {
	std::string str(s);

	if (str == "battle") {
		return EncounterType::Battle;
	}
	else if (str == "treasure") {
		return EncounterType::Treasure;
	}
	else if (str == "special_shop") {
		return EncounterType::SpecialShop;
	}
	else {
		return EncounterType::Undefined;
	}
}

static EquipType stringToEquipType(char* s) {
	std::string str(s);

	if (str == "unarmed") {
		return EquipType::Unarmed;
	}
	else if (str == "unarmed_ranged") {
		return EquipType::UnarmedRanged;
	}
	else if (str == "sword") {
		return EquipType::Sword;
	}
	else if (str == "sword2h") {
		return EquipType::Sword2H;
	}
	else if (str == "axe") {
		return EquipType::Axe;
	}
	else if (str == "axe2h") {
		return EquipType::Axe2H;
	}
	else if (str == "mace") {
		return EquipType::Mace;
	}
	else if (str == "mace2h") {
		return EquipType::Mace2H;
	}
	else if (str == "dagger") {
		return EquipType::Dagger;
	}
	else if (str == "spear") {
		return EquipType::Spear;
	}
	else if (str == "bow") {
		return EquipType::Bow;
	}
	else if (str == "wand") {
		return EquipType::Wand;
	}
	else if (str == "staff") {
		return EquipType::Staff;
	}
	else if (str == "shield") {
		return EquipType::Shield;
	}
	else if (str == "off_hand") {
		return EquipType::OffHand;
	}
	else if (str == "light_head") {
		return EquipType::LightHead;
	}
	else if (str == "light_body") {
		return EquipType::LightBody;
	}
	else if (str == "light_hands") {
		return EquipType::LightHands;
	}
	else if (str == "light_legs") {
		return EquipType::LightLegs;
	}
	else if (str == "light_feet") {
		return EquipType::LightFeet;
	}
	else if (str == "med_head") {
		return EquipType::MediumHead;
	}
	else if (str == "med_body") {
		return EquipType::MediumBody;
	}
	else if (str == "med_hands") {
		return EquipType::MediumHands;
	}
	else if (str == "med_legs") {
		return EquipType::MediumLegs;
	}
	else if (str == "med_feet") {
		return EquipType::MediumFeet;
	}
	else if (str == "heavy_head") {
		return EquipType::HeavyHead;
	}
	else if (str == "heavy_body") {
		return EquipType::HeavyBody;
	}
	else if (str == "heavy_hands") {
		return EquipType::HeavyHands;
	}
	else if (str == "heavy_legs") {
		return EquipType::HeavyLegs;
	}
	else if (str == "heavy_feet") {
		return EquipType::HeavyFeet;
	}
	else if (str == "belt") {
		return EquipType::Belt;
	}
	else if (str == "neck") {
		return EquipType::Neck;
	}
	else if (str == "ring") {
		return EquipType::Ring;
	}
	else if (str == "trinket") {
		return EquipType::Trinket;
	}
	else if (str == "flask") {
		return EquipType::Trinket;
	}
	else {
		std::cout << "EquipType Error: Could not parse " << str << ".\n";
		return EquipType::Undefined;
	}
}

static std::vector<EquipType> stringToEquipTypes(std::vector<std::string>& strv) {
	std::vector<EquipType> equipTypes;

	for (size_t i = 0; i < strv.size(); i++) {
		if (strv[i] == "sword") {
			equipTypes.push_back(EquipType::Sword);
		}
		else if (strv[i] == "sword2h") {
			equipTypes.push_back(EquipType::Sword2H);
		}
		else if (strv[i] == "axe") {
			equipTypes.push_back(EquipType::Axe);
		}
		else if (strv[i] == "axe2h") {
			equipTypes.push_back(EquipType::Axe2H);
		}
		else if (strv[i] == "mace") {
			equipTypes.push_back(EquipType::Mace);
		}
		else if (strv[i] == "mace2h") {
			equipTypes.push_back(EquipType::Mace2H);
		}
		else if (strv[i] == "dagger") {
			equipTypes.push_back(EquipType::Dagger);
		}
		else if (strv[i] == "spear") {
			equipTypes.push_back(EquipType::Spear);
		}
		else if (strv[i] == "bow") {
			equipTypes.push_back(EquipType::Bow);
		}
		else if (strv[i] == "wand") {
			equipTypes.push_back(EquipType::Wand);
		}
		else if (strv[i] == "staff") {
			equipTypes.push_back(EquipType::Staff);
		}
		else {
			std::cout << "EquipType Error: Could not parse " << strv[i] << ".\n";
		}
	}

	return equipTypes;
}

static ItemRarity stringToItemRarity(char* s) {
	std::string str(s);

	if (str == "common") {
		return ItemRarity::Common;
	}
	else if (str == "magical") {
		return ItemRarity::Magical;
	}
	else if (str == "rare") {
		return ItemRarity::Rare;
	}
	else if (str == "epic") {
		return ItemRarity::Epic;
	}
	else if (str == "fixed_artifact") {
		return ItemRarity::FixedArtifact;
	}
	else if (str == "random_artifact") {
		return ItemRarity::RandomArtifact;
	}
	else {
		std::cout << "ItemRarity Error: Could not parse " << str << ".\n";
		return ItemRarity::Undefined;
	}
}

static ItemType stringToItemType(char* s) {
	std::string str(s);

	if (str == "weapon") {
		return ItemType::Weapon;
	}
	else if (str == "armor") {
		return ItemType::Armor;
	}
	else if (str == "consumable") {
		return ItemType::Consumable;
	}
	else {
		std::cout << "ItemType Error: Could not parse " << str << ".\n";
		return ItemType::Undefined;
	}
}

static StatModType stringToStatModType(char* s) {
	std::string str(s);
	if (str == "hp") {
		return StatModType::HP;
	}
	else if (str == "hp_multi") {
		return StatModType::HPMulti;
	}
	else if (str == "hp_per_level") {
		return StatModType::HPPerLevel;
	}
	else if (str == "hp_leech") {
		return StatModType::HPLeech;
	}
	else if (str == "mp") {
		return StatModType::MP;
	}
	else if (str == "mp_multi") {
		return StatModType::MPMulti;
	}
	else if (str == "sp") {
		return StatModType::SP;
	}
	else if (str == "sp_multi") {
		return StatModType::SPMulti;
	}
	else if (str == "damage_flat") {
		return StatModType::DamageFlat;
	}
	else if (str == "damage_multi") {
		return StatModType::DamageMulti;
	}
	else if (str == "attack_power") {
		return StatModType::AttackPower;
	}
	else if (str == "attack_power_multi") {
		return StatModType::AttackPowerMulti;
	}
	else if (str == "spell_power") {
		return StatModType::SpellPower;
	}
	else if (str == "spell_power_multi") {
		return StatModType::SpellPowerMulti;
	}
	else if (str == "healing_power") {
		return StatModType::HealingPower;
	}
	else if (str == "healing_power_multi") {
		return StatModType::HealingPowerMulti;
	}
	else if (str == "accuracy") {
		return StatModType::Accuracy;
	}
	else if (str == "crit_chance") {
		return StatModType::CritChance;
	}
	else if (str == "crit_power") {
		return StatModType::CritPower;
	}
	else if (str == "double_strike_chance") {
		return StatModType::DoubleStrikeChance;
	}
	else if (str == "haste") {
		return StatModType::Haste;
	}
	else if (str == "armor") {
		return StatModType::Armor;
	}
	else if (str == "spell_defense") {
		return StatModType::SpellDefense;
	}
	else if (str == "evasion") {
		return StatModType::Haste;
	}
	else if (str == "damage_taken_flat") {
		return StatModType::DamageTakenFlat;
	}
	else if (str == "damage_taken_multi") {
		return StatModType::DamageTakenMulti;
	}
	else if (str == "resistance") {
		return StatModType::Resistance;
	}
	else {
		std::cout << "StatModType Error: Could not parse " << str << ".\n";
		return StatModType::Undefined;
	}
}

static TargetType stringToTargetType(char* str) {
	std::string string(str);
	if (string == "user") {
		return TargetType::User;
	}
	else if (string == "ally") {
		return TargetType::Ally;
	}
	else if (string == "enemy") {
		return TargetType::Enemy;
	}
	else {
		std::cout << "TargetType Error: Could not parse " << str << ".\n";
		return TargetType::Undefined;
	}
}

static TargetScope stringToTargetScope(char* str) {
	std::string string(str);
	if (string == "single") {
		return TargetScope::Single;
	}
	else if (string == "row") {
		return TargetScope::Row;
	}
	else {
		std::cout << "TargetScope Error: Could not parse " << str << ".\n";
		return TargetScope::Undefined;
	}
}

GameData::GameData() {
	
}

void GameData::LoadData() {
	sqlite3* db;
	int error = 0;

	error = sqlite3_open_v2("dsa_data.db", &db, SQLITE_OPEN_READONLY, NULL);
	if (error == SQLITE_OK) {
		std::cout << "Database successfully opened.\n";
		char* errorMessage = 0;
		std::string query;

		// Ability
		query = "SELECT COUNT(*) FROM Ability;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeAbilities, 0, &errorMessage);

		query = "SELECT * FROM Ability;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAbilities, 0, &errorMessage);

		query = "SELECT * FROM AbilityStatMod;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAbilityStatMods, 0, &errorMessage);

		// Aura
		query = "SELECT COUNT(*) FROM Aura;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeAuras, 0, &errorMessage);

		query = "SELECT * FROM Aura;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAuras, 0, &errorMessage);

		query = "SELECT * FROM AuraStatMod;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAuraStatMods, 0, &errorMessage);

		// Encounters
		query = "SELECT COUNT(*) FROM Encounter;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeEncounters, 0, &errorMessage);

		query = "SELECT * FROM Encounter;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadEncounters, 0, &errorMessage);

		query = "SELECT * FROM EncounterType;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadEncounterTypes, 0, &errorMessage);

		// Items
		query = "SELECT COUNT(*) FROM Item;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeItems, 0, &errorMessage);

		query = "SELECT * FROM Item;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadItems, 0, &errorMessage);

		query = "SELECT * FROM ItemStatMod;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadItemStatMods, 0, &errorMessage);

		// Monster
		query = "SELECT COUNT(*) FROM Monster;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeMonsters, 0, &errorMessage);

		query = "SELECT * FROM Monster;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsters, 0, &errorMessage);

		query = "SELECT * FROM MonsterAbility;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsterAbilities, 0, &errorMessage);

		query = "SELECT * FROM MonsterAI;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsterAI, 0, &errorMessage);

		query = "SELECT * FROM MonsterStatMod;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsterStatMods, 0, &errorMessage);

		// Race
		query = "SELECT COUNT(*) FROM Race;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeRaces, 0, &errorMessage);

		query = "SELECT * FROM Race;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadRaces, 0, &errorMessage);

		query = "SELECT * FROM RaceStatMod;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadRaceStatMods, 0, &errorMessage);

		// Memory Debug
		size_t bytes = 0;
		bytes += sizeof(abilities[0]) * abilities.size() + sizeof(abilities);
		bytes += sizeof(auras[0]) * auras.size() + sizeof(auras);
		bytes += sizeof(encounters[0]) * encounters.size() + sizeof(encounters);
		bytes += sizeof(encounterTypeWeights[0]) * encounterTypeWeights.size() + sizeof(encounterTypeWeights);
		bytes += sizeof(items[0]) * items.size() + sizeof(items);
		bytes += sizeof(monsters[0]) * monsters.size() + sizeof(monsters);
		bytes += sizeof(races[0]) * races.size() + sizeof(races);

		std::cout << "Memory size of game data: " << bytes << " Bytes\n";
	}
	else {
		std::cout << "An error has occured opening the database. ERROR " << error << ": " << sqlite3_errmsg(db);
	}

	std::cout << "Closing connection to the database...\n";
	sqlite3_close(db);
	std::cout << "Connection closed.";
}

AbilityData* GameData::GetAbility(size_t id) {
	if (id < abilities.size()) {
		return &abilities[id];
	}
	return nullptr;
}

AuraData* GameData::GetAura(size_t id) {
	if (id < auras.size()) {
		return &auras[id];
	}
	return nullptr;
}

EncounterData* GameData::GetEncounter(size_t id) {
	if (id < encounters.size()) {
		return &encounters[id];
	}
	return nullptr;
}

EncounterData* GameData::GetEncounterRandom(int level, std::mt19937_64& mt) {
	int sum = 0;
	std::vector<std::pair<EncounterData, size_t>> possible;
	
	for (size_t i = 0; i < encounters.size(); i++) {
		if (encounters[i].Weight > 0 && level >= encounters[i].MinLevel && level <= encounters[i].MaxLevel) {
			sum += encounters[i].Weight;
			possible.push_back({ encounters[i], i });
		}
	}

	int roll = Random::RandomInt(1, sum);

	for (size_t i = 0; i < possible.size(); i++) {
		roll -= possible[i].first.Weight;
		if (roll <= 0) {
			return &encounters[possible[i].second];
		}
	}

	return nullptr;
}

EncounterType GameData::GetEncounterTypeRandom(std::mt19937_64& mt) {
	int sum = 0;

	for (size_t i = 0; i < encounterTypeWeights.size(); i++) {
		sum += encounterTypeWeights[i].second;
	}

	int roll = Random::RandomInt(1, sum, mt);

	for (size_t i = 0; i < encounterTypeWeights.size(); i++) {
		roll -= encounterTypeWeights[i].second;
		if (roll <= 0) {
			return encounterTypeWeights[i].first;
		}
	}

	return EncounterType::Undefined;
}

ItemData* GameData::GetItem(size_t id) {
	if (id < items.size()) {
		return &items[id];
	}
	return nullptr;
}

MonsterData* GameData::GetMonster(size_t id) {
	if (id < monsters.size()) {
		return &monsters[id];
	}
	return nullptr;
}

RaceData* GameData::GetRace(size_t id) {
	if (id < races.size()) {
		return &races[id];
	}
	return nullptr;
}

int GameData::resizeAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	abilities.reserve(count);

	std::cout << "Loading " << count << " abilities...\n";

	return 0;
}

int GameData::loadAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	AbilityData ad;

	ad.ID = std::stoull(data[0]);
	ad.Name = data[1];
	ad.Icon = static_cast<std::string>(data[2]) + ".png";
	ad.Description = data[3];
	ad.IsPassive = (std::stoi(data[4]) == 1);

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		ad.Categories = stringToCategory(strv);
	}

	if (data[6] != nullptr) {
		strv = splitString(data[6], ',');
		ad.Elements = stringToElements(strv);
	}

	if (data[7] != nullptr) {
		strv = splitString(data[7], ',');
		ad.RequiredWeaponTypes = stringToEquipTypes(strv);
	}

	if (data[8] != nullptr) {
		ad.TargetType = stringToTargetType(data[8]);
	}

	if (data[9] != nullptr) {
		ad.TargetScope = stringToTargetScope(data[9]);
	}

	ad.IsRanged = (std::stoi(data[10]) == 1);
	ad.UseTime = std::stoi(data[11]);
	ad.Cooldown = std::stoi(data[12]);
	ad.MaxCharges = std::stoi(data[13]);
	ad.MPCost = std::stoll(data[14]);
	ad.SPCost = std::stoll(data[15]);
	ad.BaseHitChance = std::stoi(data[16]);
	ad.CanDodge = (std::stoi(data[17]) == 1);
	ad.CanBlock = (std::stoi(data[18]) == 1);
	ad.CanCrit = (std::stoi(data[19]) == 1);
	ad.CanDoubleStrike = (std::stoi(data[20]) == 1);
	ad.OffHandAttack = (std::stoi(data[21]) == 1);

	if (data[22] != nullptr) {
		strv = splitString(data[22], ';');
		for (size_t i = 0; i < strv.size(); i++) {
			ad.Effects.push_back(stringToAbilityEffect(strv[i]));
		}
	}

	abilities.push_back(ad);

	std::cout << "Loaded " << data[1] << " into ability vector.\n";

	return 0;
}

int GameData::loadAbilityStatMods(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	StatMod sm;
	size_t abilityID = std::stoull(data[1]);

	sm.StatModType = stringToStatModType(data[2]);
	sm.Value = std::stoll(data[3]);

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		sm.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		sm.Elements = stringToElements(strv);
	}

	sm.AbilityID = std::stoull(data[6]);
	sm.AuraID = std::stoull(data[7]);

	abilities[abilityID].StatMods.push_back(sm);

	return 0;
}

int GameData::resizeAuras(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	auras.reserve(count);

	std::cout << "Loading " << count << " auras...\n";

	return 0;
}

int GameData::loadAuras(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	AuraData ad;

	ad.ID = std::stoull(data[0]);
	ad.Name = data[1];
	ad.Icon = static_cast<std::string>(data[2]) + ".png";
	ad.Description = data[3];

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		ad.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		ad.Elements = stringToElements(strv);
	}

	ad.BaseDuration = std::stoi(data[6]);
	ad.MaxStacks = std::stoi(data[7]);
	ad.TickRate = std::stoi(data[8]);
	ad.IsBuff = (std::stoi(data[9]) == 1);
	ad.IsHidden = (std::stoi(data[10]) == 1);
	ad.IsUnique = (std::stoi(data[11]) == 1);
	ad.IsUniqueByActor = (std::stoi(data[12]) == 1);
	ad.ConsumeOnUse = (std::stoi(data[13]) == 1);
	ad.StacksExpireOneByOne = (std::stoi(data[14]) == 1);
	ad.StacksConsumeOneByOne = (std::stoi(data[15]) == 1);
	ad.MultiplyStatModsByStacks = (std::stoi(data[16]) == 1);
	ad.IsStun = (std::stoi(data[17]) == 1);
	ad.IsDisarm = (std::stoi(data[18]) == 1);
	ad.IsSilence = (std::stoi(data[19]) == 1);
	ad.CanCrit = (std::stoi(data[20]) == 1);

	if (data[21] != nullptr) {
		strv = splitString(data[21], ';');
		for (size_t i = 0; i < strv.size(); i++) {
			ad.OnTick.push_back(stringToAbilityEffect(strv[i]));
		}
	}

	if (data[22] != nullptr) {
		strv = splitString(data[22], ';');
		for (size_t i = 0; i < strv.size(); i++) {
			ad.OnExpiry.push_back(stringToAbilityEffect(strv[i]));
		}
	}

	auras.push_back(ad);

	std::cout << "Loaded " << data[1] << " into aura vector.\n";

	return 0;
}

int GameData::loadAuraStatMods(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	StatMod sm;
	size_t auraID = std::stoull(data[1]);

	sm.StatModType = stringToStatModType(data[2]);
	sm.Value = std::stoll(data[3]);

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		sm.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		sm.Elements = stringToElements(strv);
	}

	sm.AbilityID = std::stoull(data[6]);
	sm.AuraID = std::stoull(data[7]);

	auras[auraID].StatMods.push_back(sm);

	return 0;
}

int GameData::resizeEncounters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	encounters.reserve(count);

	std::cout << "Loading " << count << " encounters...\n";

	return 0;
}

int GameData::loadEncounters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	EncounterData ed;

	ed.Name = data[1];
	ed.IsUnique = (std::stoi(data[2]) == 1);
	ed.MinLevel = std::stoi(data[3]);
	ed.MaxLevel = std::stoi(data[4]);
	ed.Weight = std::stoi(data[5]);

	if (data[6] != nullptr) {
		ed.FrontFarLeftMonster = std::stoull(data[6]);
	}

	if (data[7] != nullptr) {
		ed.FrontLeftMonster = std::stoull(data[7]);
	}

	if (data[8] != nullptr) {
		ed.FrontCenterMonster = std::stoull(data[8]);
	}

	if (data[9] != nullptr) {
		ed.FrontRightMonster = std::stoull(data[9]);
	}

	if (data[10] != nullptr) {
		ed.FrontFarRightMonster = std::stoull(data[10]);
	}

	if (data[11] != nullptr) {
		ed.BackFarLeftMonster = std::stoull(data[11]);
	}

	if (data[12] != nullptr) {
		ed.BackLeftMonster = std::stoull(data[12]);
	}

	if (data[13] != nullptr) {
		ed.BackCenterMonster = std::stoull(data[13]);
	}

	if (data[14] != nullptr) {
		ed.BackRightMonster = std::stoull(data[14]);
	}

	if (data[15] != nullptr) {
		ed.BackFarRightMonster = std::stoull(data[15]);
	}

	encounters.push_back(ed);

	std::cout << "Loaded " << data[1] << " into encounter vector.\n";

	return 0;
}

int GameData::loadEncounterTypes(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::pair<EncounterType, int> etw{};

	etw.first = stringToEncounterType(data[1]);
	etw.second = std::stoi(data[2]);

	encounterTypeWeights.push_back(etw);

	return 0;
}

int GameData::resizeItems(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	items.reserve(count);

	std::cout << "Loading " << count << " items...\n";

	return 0;
}

int GameData::loadItems(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	ItemData id;

	id.Name = data[1];
	id.ItemRarity = stringToItemRarity(data[2]);
	id.BaseValue = std::stoll(data[3]);
	id.ItemType = stringToItemType(data[4]);

	if (data[5] != nullptr) {
		id.EquipType = stringToEquipType(data[5]);
	}
	id.WeaponDamage = std::stoll(data[6]);
	id.WeaponMultiplier = std::stoll(data[7]);
	id.WeaponSpeed = std::stoll(data[8]);
	id.WeaponHitChance = std::stoll(data[9]);

	if (data[10] != nullptr) {
		strv = splitString(data[10], ',');
		id.WeaponElements = stringToElements(strv);
	}
	id.Armor = std::stoll(data[11]);
	id.SpellDefense = std::stoll(data[12]);

	items.push_back(id);

	std::cout << "Loaded " << data[1] << " into item vector.\n";

	return 0;
}

int GameData::loadItemStatMods(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	StatMod sm;
	size_t itemID = std::stoull(data[1]);

	sm.StatModType = stringToStatModType(data[2]);
	sm.Value = std::stoll(data[3]);

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		sm.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		sm.Elements = stringToElements(strv);
	}

	sm.AbilityID = std::stoull(data[6]);
	sm.AuraID = std::stoull(data[7]);

	items[itemID].StatMods.push_back(sm);

	return 0;
}

int GameData::resizeMonsters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	monsters.reserve(count);

	std::cout << "Loading " << count << " monsters...\n";

	return 0;
}

int GameData::loadMonsters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	MonsterData md;

	md.Name = data[1];

	if (data[2] != nullptr) {
		md.Title = data[2];
	}

	md.Graphic = data[3];
	md.IsUnique = (std::stoi(data[4]) == 1);
	md.Level = std::stoi(data[5]);
	md.BaseHP = std::stoll(data[6]);
	md.BaseMP = std::stoll(data[7]);
	md.BaseSP = std::stoll(data[8]);
	md.BaseAttackPower = std::stoll(data[9]);
	md.BaseSpellPower = std::stoll(data[10]);
	md.BaseHealingPower = std::stoll(data[11]);
	md.BaseArmor = std::stoll(data[12]);
	md.BaseSpellDefense = std::stoll(data[13]);
	md.BaseEvasion = std::stoll(data[14]);
	md.BaseHitChance = std::stoll(data[15]);
	md.BaseAccuracy = std::stoll(data[16]);
	md.AttackSpeed = std::stoi(data[17]);

	if (data[18] != nullptr) {
		md.MainHandWeaponType = stringToEquipType(data[18]);
	}

	md.MainHandWeaponDamage = std::stoll(data[19]);
	md.MainHandMultiplier = std::stoll(data[20]);

	if (data[21] != nullptr) {
		md.OffHandWeaponType = stringToEquipType(data[21]);
	}

	md.OffHandWeaponDamage = std::stoll(data[22]);
	md.OffHandMultiplier = std::stoll(data[23]);
	md.EXPDrop = std::stoll(data[24]);
	md.GoldDrop = std::stoll(data[25]);
	md.LootPoints = std::stoll(data[26]);

	monsters.push_back(md);

	std::cout << "Loaded " << data[1] << " into monster vector.\n";

	return 0;
}

int GameData::loadMonsterAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	size_t monsterID = std::stoull(data[1]);
	monsters[monsterID].AbilityIDs.push_back(std::stoull(data[2]));

	return 0;
}

int GameData::loadMonsterAI(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	Tactic t;
	size_t monsterID = std::stoull(data[1]);

	t.Rating = std::stoull(data[2]);
	t.Condition = stringToAICondition(data[3]);
	t.ConditionValue = std::stoll(data[4]);
	t.Target = stringToAITarget(data[5]);
	t.TargetValue = std::stoll(data[6]);
	t.AbilityID = std::stoull(data[7]);
	t.Weight = std::stoi(data[8]);

	monsters[monsterID].Tactics.push_back(t);

	return 0;
}

int GameData::loadMonsterStatMods(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	StatMod sm;
	size_t monsterID = std::stoull(data[1]);

	sm.StatModType = stringToStatModType(data[2]);
	sm.Value = std::stoll(data[3]);

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		sm.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		sm.Elements = stringToElements(strv);
	}

	sm.AbilityID = std::stoull(data[6]);
	sm.AuraID = std::stoull(data[7]);

	monsters[monsterID].StatMods.push_back(sm);

	return 0;
}

int GameData::resizeRaces(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoll(data[0]);
	races.reserve(count);

	std::cout << "Loading " << count << " races...\n";

	return 0;
}

int GameData::loadRaces(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	RaceData rd;

	rd.Name = data[1];
	rd.BaseHP = std::stoll(data[2]);
	rd.BaseMP = std::stoll(data[3]);
	rd.BaseSP = std::stoll(data[4]);
	rd.BaseEXPPerLevel = std::stoll(data[5]);

	std::cout << "Loaded " << data[1] << " into race vector.\n";

	races.push_back(rd);

	return 0;
}

int GameData::loadRaceStatMods(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	StatMod sm;
	size_t raceID = std::stoull(data[1]);

	sm.StatModType = stringToStatModType(data[2]);
	sm.Value = std::stoll(data[3]);

	if (data[4] != nullptr) {
		strv = splitString(data[4], ',');
		sm.Categories = stringToCategory(strv);
	}

	if (data[5] != nullptr) {
		strv = splitString(data[5], ',');
		sm.Elements = stringToElements(strv);
	}

	sm.AbilityID = std::stoull(data[6]);
	sm.AuraID = std::stoull(data[7]);

	races[raceID].StatMods.push_back(sm);

	return 0;
}