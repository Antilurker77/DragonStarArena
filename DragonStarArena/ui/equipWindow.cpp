// ================================================================
//
// equipWindow.cpp
//
// ================================================================

#include "equipWindow.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../data/item.hpp"
#include "../data/id/equipType.hpp"
#include "../data/id/itemType.hpp"
#include "../entity/actor.hpp"

EquipWindow::EquipWindow() {
	background.setSize(sf::Vector2f(800.f, 450.f));
	background.setFillColor(sf::Color(0, 0, 0, 191));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));
	background.setPosition(settings.ScreenWidthF / 2.f - 400.f, settings.ScreenHeightF / 2.f - 225.f);

	auto bgPos = background.getPosition();
	bgPos.y -= 40.f;

	for (size_t i = 0; i < playerButtons.size(); i++) {
		playerButtons[i].SetPosition(bgPos);
		bgPos.x += 133.f;
	}

	for (size_t i = 0; i < equipBG.size(); i++) {
		equipBG[i].setSize(sf::Vector2f(equipBGSize, equipBGSize));
		equipBG[i].setFillColor(sf::Color(0, 0, 0, 255));
		equipBG[i].setOutlineThickness(1.f);
		equipBG[i].setOutlineColor(sf::Color(255, 255, 255, 255));
	}

	for (size_t i = 0; i < equipIcons.size(); i++) {
		equipIcons[i].SetSpriteCount(1);
		equipIcons[i].SetScale(2.f);
		equipIcons[i].SetColor(sf::Color(127, 127, 127, 255));
		equipIcons[i].SetTexture("gfx/icon/item/placeholder.png");
	}

	bgPos = background.getPosition();

	// Slot  0: Main-Hand
	// Slot  1: Off-Hand
	// Slot  2: Head
	// Slot  3: Body
	// Slot  4: Hands
	// Slot  5: Legs
	// Slot  6: Feet
	// Slot  7: Belt
	// Slot  8: Neck
	// Slot  9: Ring 1
	// Slot 10: Ring 2
	// Slot 11: Trinket
	equipBG[0].setPosition(bgPos.x + 60.f, bgPos.y + 200.f);
	equipIcons[0].Move(sf::Vector2f(bgPos.x + 60.f, bgPos.y + 200.f));
	equipIcons[0].SetTexture("gfx/icon/item/sword.png");

	equipBG[1].setPosition(bgPos.x + 100.f, bgPos.y + 200.f);
	equipIcons[1].Move(sf::Vector2f(bgPos.x + 100.f, bgPos.y + 200.f));
	equipIcons[1].SetTexture("gfx/icon/item/shield.png");

	equipBG[2].setPosition(bgPos.x + 20.f, bgPos.y + 20.f);
	equipIcons[2].Move(sf::Vector2f(bgPos.x + 20.f, bgPos.y + 20.f));
	equipIcons[2].SetTexture("gfx/icon/item/head.png");

	equipBG[3].setPosition(bgPos.x + 20.f, bgPos.y + 60.f);
	equipIcons[3].Move(sf::Vector2f(bgPos.x + 20.f, bgPos.y + 60.f));
	equipIcons[3].SetTexture("gfx/icon/item/body.png");

	equipBG[4].setPosition(bgPos.x + 20.f, bgPos.y + 100.f);
	equipIcons[4].Move(sf::Vector2f(bgPos.x + 20.f, bgPos.y + 100.f));
	equipIcons[4].SetTexture("gfx/icon/item/hands.png");

	equipBG[5].setPosition(bgPos.x + 20.f, bgPos.y + 140.f);
	equipIcons[5].Move(sf::Vector2f(bgPos.x + 20.f, bgPos.y + 140.f));
	equipIcons[5].SetTexture("gfx/icon/item/legs.png");

	equipBG[6].setPosition(bgPos.x + 20.f, bgPos.y + 180.f);
	equipIcons[6].Move(sf::Vector2f(bgPos.x + 20.f, bgPos.y + 180.f));
	equipIcons[6].SetTexture("gfx/icon/item/feet.png");

	equipBG[7].setPosition(bgPos.x + 140.f, bgPos.y + 20.f);
	equipIcons[7].Move(sf::Vector2f(bgPos.x + 140.f, bgPos.y + 20.f));
	equipIcons[7].SetTexture("gfx/icon/item/belt.png");

	equipBG[8].setPosition(bgPos.x + 140.f, bgPos.y + 60.f);
	equipIcons[8].Move(sf::Vector2f(bgPos.x + 140.f, bgPos.y + 60.f));
	equipIcons[8].SetTexture("gfx/icon/item/neck.png");

	equipBG[9].setPosition(bgPos.x + 140.f, bgPos.y + 100.f);
	equipIcons[9].Move(sf::Vector2f(bgPos.x + 140.f, bgPos.y + 100.f));
	equipIcons[9].SetTexture("gfx/icon/item/ring.png");

	equipBG[10].setPosition(bgPos.x + 140.f, bgPos.y + 140.f);
	equipIcons[10].Move(sf::Vector2f(bgPos.x + 140.f, bgPos.y + 140.f));
	equipIcons[10].SetTexture("gfx/icon/item/ring.png");

	equipBG[11].setPosition(bgPos.x + 140.f, bgPos.y + 180.f);
	equipIcons[11].Move(sf::Vector2f(bgPos.x + 140.f, bgPos.y + 180.f));
	equipIcons[11].SetTexture("gfx/icon/item/trinket.png");

	inventoryBG.setSize(sf::Vector2f(350.f, 300.f));
	inventoryBG.setFillColor(sf::Color(0, 0, 0, 255));
	inventoryBG.setOutlineThickness(1.f);
	inventoryBG.setOutlineColor(sf::Color(255, 255, 255, 255));
	inventoryBG.setPosition(bgPos.x + 425.f, bgPos.y + 75.f);
}

void EquipWindow::Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick) {
	sf::Vector2f mousePosF{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	displayTooltip = false;

	for (size_t i = 0; i < playerButtons.size(); i++) {
		if (playerButtons[i].Update(secondsPerUpdate, mousePos)) {
			if (leftClick) {
				viewedPlayer = i;
				setEquipment(i);
			}
		}
	}

	for (size_t i = 0; i < equipBG.size(); i++) {
		if (equipBG[i].getGlobalBounds().contains(mousePosF) && !equippedItems->at(i).IsNull()) {
			displayTooltip = true;
			tooltip.SetTooltip(&equippedItems->at(i));
			auto size = tooltip.GetSize();
			tooltip.SetPosition(mousePosF.x, mousePosF.y - size.y);
		}
	}

	for (size_t i = 0; i < highlightBoxes.size(); i++) {
		if (highlightBoxes[i].getGlobalBounds().contains(mousePosF)) {
			highlightBoxes[i].setFillColor(sf::Color(127, 127, 127, 191));
			displayTooltip = true;
			tooltip.SetTooltip(&inventory->at(displayedItems[i]));
			auto size = tooltip.GetSize();
			tooltip.SetPosition(mousePosF.x, mousePosF.y - size.y);
			if (rightClick) {
				Item item = inventory->at(displayedItems[i]); // need to do it like this, weird memory issue
				players->at(viewedPlayer)->Equip(item, inventory, displayedItems[i]);
				setEquipment(viewedPlayer);
				filterInventory();
			}
		}
		else {
			highlightBoxes[i].setFillColor(sf::Color(127, 127, 127, 0));
		}
	}
}

void EquipWindow::Render(sf::RenderTarget& window) {
	window.draw(background);

	for (size_t i = 0; i < equipBG.size(); i++) {
		window.draw(equipBG[i]);
	}

	for (size_t i = 0; i < equipIcons.size(); i++) {
		equipIcons[i].Render(window, 0.f);
	}

	for (size_t i = 0; i < playerButtons.size(); i++) {
		playerButtons[i].Render(window);
	}

	window.draw(inventoryBG);

	for (size_t i = 0; i < highlightBoxes.size(); i++) {
		window.draw(highlightBoxes[i]);
	}

	for (size_t i = 0; i < inventoryIcons.size(); i++) {
		inventoryIcons[i].Render(window, 0.f);
	}

	for (size_t i = 0; i < inventoryText.size(); i++) {
		window.draw(inventoryText[i]);
	}

	if (displayTooltip) {
		tooltip.Render(window);
	}
}

void EquipWindow::SetPlayerList(std::vector<ActorPtr>* list) {
	players = list;

	for (size_t i = 0; i < players->size(); i++) {
		playerButtons[i].SetString(players->at(i)->GetName(), 16u);
	}

	setEquipment(viewedPlayer);
}

void EquipWindow::SetInventoryList(std::vector<Item>* list) {
	inventory = list;

	filterInventory();
}

void EquipWindow::setEquipment(size_t index) {
	equippedItems = players->at(index)->GetEquipment();

	for (size_t i = 0; i < equipIcons.size(); i++) {
		if (!equippedItems->at(i).IsNull()) {
			equipIcons[i].SetColor(sf::Color(255, 255, 255, 255));
			equipIcons[i].SetTexture(equippedItems->at(i).GetIcon());
		}
		else {
			equipIcons[i].SetColor(sf::Color(127, 127, 127, 255));
		}
	}
}

void EquipWindow::filterInventory() {
	inventoryText.clear();
	inventoryIcons.clear();
	highlightBoxes.clear();
	displayedItems.clear();

	for (size_t i = 0; i < inventory->size(); i++) {
		if (inventory->at(i).GetItemType() == ItemType::Weapon || inventory->at(i).GetItemType() == ItemType::Armor) {
			if (currentFilter == EquipType::Undefined || inventory->at(i).GetEquipType() == currentFilter) {
				displayedItems.push_back(i);
			}
		}
	}

	float iFloat = 0.f;
	auto pos = inventoryBG.getPosition();
	pos.x += 2.f;
	pos.y += 2.f;

	for (size_t i = 0; i < displayedItems.size(); i++) {
		Item* item = &inventory->at(displayedItems[i]);
		sfe::RichText text;
		Entity icon;
		sf::RectangleShape box;

		text.setString(DataString::ItemRarityColorCode(item->GetItemRarity()) + item->GetName());
		text.setCharacterSize(16u);
		text.setFont(*assetManager.LoadFont(settings.Font));

		// todo: rarity color
		icon.SetSpriteCount(1);
		icon.SetTexture(item->GetIcon());

		box.setSize(sf::Vector2f(348.f, 20.f));
		box.setFillColor(sf::Color(127, 127, 127, 0));

		icon.Move(sf::Vector2f(pos.x, pos.y + (iFloat * 20.f)));
		text.setPosition(pos.x + 18.f, pos.y + (iFloat * 20.f) - 2.f);
		box.setPosition(pos.x - 1.f, pos.y + (iFloat * 20.f) - 1.f);

		inventoryIcons.push_back(icon);
		inventoryText.push_back(text);
		highlightBoxes.push_back(box);

		iFloat += 1.f;
	}
}