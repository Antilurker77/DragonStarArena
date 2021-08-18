// ================================================================
//
// inventoryList.cpp
//
// ================================================================

#include "inventoryList.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../data/item.hpp"

InventoryList::InventoryList() {
	titleText.setString("Inventory List");
	titleText.setFont(*assetManager.LoadFont(settings.Font));
	titleText.setCharacterSize(24u);

	background.setFillColor(sf::Color(0, 0, 0, 191));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));
}

void InventoryList::Update(float secondsPerUpdate, sf::Vector2i mousePos) {
	sf::Vector2f mousePosF{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	displayTooltip = false;

	for (size_t i = 0; i < selectionBoxes.size(); i++) {
		if (selectionBoxes[i].getGlobalBounds().contains(mousePosF)) {
			selectionBoxes[i].setFillColor(sf::Color(127, 127, 127, 191));
			tooltip.SetTooltip(&inventory->at(i));
			auto tooltipSize = tooltip.GetSize();
			tooltip.SetPosition(selectionBoxes[i].getPosition().x, selectionBoxes[i].getPosition().y - tooltipSize.y);
			displayTooltip = true;
		}
		else {
			selectionBoxes[i].setFillColor(sf::Color(127, 127, 127, 0));
		}
	}
}

void InventoryList::Render(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(titleText);

	for (size_t i = 0; i < itemIcons.size(); i++) {
		window.draw(selectionBoxes[i]);
		itemIcons[i].Render(window, 0.f);
		window.draw(itemText[i]);
	}

	if (displayTooltip) {
		tooltip.Render(window);
	}
}

void InventoryList::SetList(std::vector<Item>& items) {
	itemIcons.clear();
	itemText.clear();
	selectionBoxes.clear();

	inventory = &items;

	float longest = titleText.getLocalBounds().width + 4.f;

	for (size_t i = 0; i < items.size(); i++) {
		sfe::RichText text;
		Entity icon;
		sf::RectangleShape box;

		// todo: different icons
		text.setString(DataString::ItemRarityColorCode(items[i].GetItemRarity()) + items[i].GetName());
		text.setCharacterSize(16u);
		text.setFont(*assetManager.LoadFont(settings.Font));

		icon.SetSpriteCount(1);
		icon.SetTexture("gfx/icon/item/placeholder.png");

		box.setFillColor(sf::Color(127, 127, 127, 0));

		float width = 16.f + 2.f + text.getLocalBounds().width;
		if (width > longest) {
			longest = width;
		}

		itemText.push_back(text);
		itemIcons.push_back(icon);
		selectionBoxes.push_back(box);
	}

	sf::Vector2f size{ longest + 2.f, 18.f };
	for (size_t i = 0; i < selectionBoxes.size(); i++) {
		selectionBoxes[i].setSize(size);
	}

	sf::Vector2f backgroundSize{ longest + 4.f, 0.f };
	backgroundSize.y = titleText.getLocalBounds().height + 2.f;
	backgroundSize.y += 12.f + (20.f * items.size()) + 2.f;
	background.setSize(backgroundSize);
}

void InventoryList::SetPosition(float x, float y) {
	SetPosition(sf::Vector2f{ x, y });
}

void InventoryList::SetPosition(sf::Vector2f pos) {
	pos.x = std::roundf(pos.x);
	pos.y = std::roundf(pos.y);
	
	background.setPosition(pos);

	titleText.setPosition(pos.x + background.getSize().x / 2.f - titleText.getLocalBounds().width / 2.f, pos.y);

	pos.x += 2.f;
	pos.y += 16.f;

	for (size_t i = 0; i < itemIcons.size(); i++) {
		pos.y += 20.f;
		
		itemIcons[i].Move(pos);
		itemText[i].setPosition(pos.x + 18.f, pos.y - 3.f);
		selectionBoxes[i].setPosition(pos.x - 1.f, pos.y - 2.f);
	}
}

sf::Vector2f InventoryList::GetSize() {
	return background.getSize();
}