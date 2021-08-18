// ================================================================
//
// tooltip.cpp
//
// ================================================================

#include "tooltip.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../data/enconterData.hpp"
#include "../data/gameData.hpp"
#include "../data/item.hpp"
#include "../data/monsterData.hpp"
#include "../data/statMod.hpp"
#include "../data/id/encounterType.hpp"
#include "../data/id/itemType.hpp"
#include "../entity/encounterNode.hpp"

Tooltip::Tooltip() {
	background.setFillColor(sf::Color(0, 0, 0, 191));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));
}

void Tooltip::Render(sf::RenderTarget& window) {
	window.draw(background);
	for (size_t i = 0; i < tooltipText.size(); i++) {
		window.draw(tooltipText[i]);
	}
}

void Tooltip::SetPosition(float x, float y) {
	SetPosition(sf::Vector2f{ x, y });
}

void Tooltip::SetPosition(sf::Vector2f pos) {
	pos.x = std::roundf(pos.x);
	pos.y = std::roundf(pos.y);
	
	background.setPosition(pos);

	pos.x += 2.f;
	pos.y += 2.f;

	for (size_t i = 0; i < tooltipText.size(); i++) {
		tooltipText[i].setPosition(pos);
		pos.y += 18.f;
	}
}

void Tooltip::SetTooltip(EncounterNode* node) {
	tooltipText.clear();
	sfe::RichText t;
	std::string str;

	// Type
	EncounterType encounterType = node->GetEncounterType();
	str = DataString::EncounterTypeString(encounterType);
	t.setString(str);
	tooltipText.push_back(t);

	// Monsters
	if (encounterType == EncounterType::Battle || encounterType == EncounterType::UniqueBattle) {
		EncounterData* encounterData = node->GetEncounterData();
		MonsterData* monsterData = nullptr;

		if (encounterData->FrontFarLeftMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->FrontFarLeftMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->FrontLeftMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->FrontLeftMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->FrontCenterMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->FrontCenterMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->FrontRightMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->FrontRightMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->FrontFarRightMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->FrontFarRightMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->BackFarLeftMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->BackFarLeftMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->BackLeftMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->BackLeftMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->BackCenterMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->BackCenterMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->BackRightMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->BackRightMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}

		if (encounterData->BackFarRightMonster != 0) {
			monsterData = gameData.GetMonster(encounterData->BackFarRightMonster);
			str = "#aaaaaa LV#default " + std::to_string(monsterData->Level) + " " + monsterData->Name;
			if (!monsterData->Title.empty()) {
				str += ", " + monsterData->Title;
			}
			t.setString(str);
			tooltipText.push_back(t);
		}
	}

	format();
}

void Tooltip::SetTooltip(Item* item) {
	tooltipText.clear();
	sfe::RichText t;
	std::string str;

	// Name
	str = DataString::ItemRarityColorCode(item->GetItemRarity()) + item->GetName();
	t.setString(str);
	tooltipText.push_back(t);

	// Item Type
	str = "#aaaaaa " + DataString::EquipTypeString(item->GetEquipType());
	t.setString(str);
	tooltipText.push_back(t);

	// Weapon Stats
	if (item->GetItemType() == ItemType::Weapon) {
		str = std::to_string(item->GetWeaponDamage()) + " + " + std::to_string(item->GetWeaponMultiplier() / 100) + "% Attack Power Damage";
		t.setString(str);
		tooltipText.push_back(t);

		str = DataString::TimeString(item->GetWeaponSpeed()) + " Use Time";
		t.setString(str);
		tooltipText.push_back(t);

		str = std::to_string(item->GetWeaponHitChance() / 100) + "% Hit Chance";
		t.setString(str);
		tooltipText.push_back(t);
	}
	// Armor Stats
	else if (item->GetItemType() == ItemType::Armor) {
		str = "";
		int64_t armor = item->GetArmor();
		int64_t spellDefense = item->GetSpellDefense();

		if (armor > 0) {
			str += std::to_string(armor) + " Armor";
			if (spellDefense > 0) {
				str += " | ";
			}
		}
		if (spellDefense > 0) {
			str += std::to_string(spellDefense) + " Spell Defense";
		}
		if (!str.empty()) {
			t.setString(str);
			tooltipText.push_back(t);
		}
	}

	// Stat Mods
	std::vector<StatMod> statMods = item->GetStatMods();

	for (size_t i = 0; i < statMods.size(); i++) {
		str = "#affix " + DataString::StatModString(&statMods[i]);
		t.setString(str);
		tooltipText.push_back(t);
	}

	format();
}

sf::Vector2f Tooltip::GetSize() {
	return background.getSize();
}

void Tooltip::format() {
	sf::Vector2f size{ 0.f, 0.f };

	float longest = 4.f;
	size.y = 4.f;

	for (size_t i = 0; i < tooltipText.size(); i++) {
		tooltipText[i].setCharacterSize(16u);
		tooltipText[i].setFont(*assetManager.LoadFont(settings.Font));

		float length = tooltipText[i].getLocalBounds().width + 5.f;
		if (length > longest) {
			longest = length;
		}
		size.y += 18.f;
	}

	size.x = longest;
	background.setSize(size);
}