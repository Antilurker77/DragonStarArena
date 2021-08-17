// ================================================================
//
// inventoryList.hpp
// 
// Window that displays the party's inventory.
//
// ================================================================

#pragma once

class Item;

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "richText.hpp"
#include "../entity/entity.hpp"

class InventoryList {
public:
	InventoryList();

	// Updates the inventory list.
	void Update(float secondsPerUpdate, sf::Vector2i mousePos);

	// Draws the inventory list.
	void Render(sf::RenderTarget& window);

	// Sets the items to display in the list.
	void SetList(std::vector<Item>& items);

	// Sets the position of the inventory list.
	void SetPosition(float x, float y);

	// Sets the position of the inventory list.
	void SetPosition(sf::Vector2f pos);

	// Returns the size of the inventory list.
	sf::Vector2f GetSize();

private:
	// Calculates the size for the background.
	//void calcSize();

	// ================================

	sfe::RichText titleText;
	sf::RectangleShape background;
	std::vector<Entity> itemIcons;
	std::vector<sfe::RichText> itemText;
	std::vector<sf::RectangleShape> selectionBoxes;
};