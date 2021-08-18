// ================================================================
//
// tooltip.cpp
//
// ================================================================

#include "tooltip.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../data/item.hpp"
#include "../data/statMod.hpp"

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

void Tooltip::SetTooltip(Item* item) {
	tooltipText.clear();
	sfe::RichText t;
	std::string str;

	// Name
	str = DataString::ItemRarityColorCode(item->GetItemRarity()) + item->GetName();
	t.setString(str);
	tooltipText.push_back(t);

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