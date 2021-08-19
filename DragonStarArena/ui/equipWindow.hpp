// ================================================================
//
// equipWindow.hpp
// 
// Window where player can equip gear and see their stats.
//
// ================================================================

#pragma once

class Actor;
class Item;
enum class EquipType;

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Window.hpp>
#include "button.hpp"
#include "richText.hpp"
#include "tooltip.hpp"
#include "../entity/entity.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class EquipWindow {
public:
	EquipWindow();

	// Updates the equip window.
	void Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick, bool draggingLeft);

	// Draws the equip window.
	void Render(sf::RenderTarget& window);

	// Sets the player reference.
	void SetPlayerList(std::vector<ActorPtr>* list);

	// Sets the inventory reference.
	void SetInventoryList(std::vector<Item>* list);

private:
	// Sets up the equipment entities.
	void setEquipment(size_t index);

	// Sets up the inventory display based on the current filter.
	void filterInventory();

	// ================================
	Tooltip tooltip;
	bool displayTooltip = false;

	sf::RectangleShape background;
	std::array<Button, 6> playerButtons;

	std::vector<ActorPtr>* players = nullptr;
	size_t viewedPlayer = 0;

	std::array<sf::RectangleShape, 12> equipBG;
	std::array<Entity, 12> equipIcons;
	const float equipBGSize = 32.f;
	std::array<Item, 12>* equippedItems = nullptr;

	sf::RectangleShape inventoryBG;
	std::vector<Item>* inventory = nullptr;
	std::vector<sfe::RichText> inventoryText{};
	std::vector<Entity> inventoryIcons{};
	std::vector<sf::RectangleShape> highlightBoxes{};
	std::vector<size_t> displayedItems{};
	EquipType currentFilter{};

	bool dragging = false;
	float holdingDownTime = 0.f;
	Entity entityOnCursor;
	size_t indexOnCursor = 0;
};