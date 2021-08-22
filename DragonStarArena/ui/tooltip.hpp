// ================================================================
//
// tooltip.hpp
// 
// UI element that displays a miniwindow describing the properties
// of a given object.
//
// ================================================================

#pragma once

class Ability;
class Actor;
class EncounterNode;
class Item;

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "richText.hpp"

class Tooltip {
public:
	Tooltip();

	// Renders the tooltip.
	void Render(sf::RenderTarget& window);

	// Sets the position of the tooltip.
	void SetPosition(float x, float y);

	// Sets the position of the tooltip.
	void SetPosition(sf::Vector2f pos);

	// Sets the tooltip to display information about the given ability.
	void SetTooltip(Ability* ability, Actor* actor = nullptr);

	// Sets the tooltip to display information about the given node.
	void SetTooltip(EncounterNode* node);

	// Sets the tooltip to display information about the given item.
	void SetTooltip(Item* item);

	// Returns the size of the tooltip.
	sf::Vector2f GetSize();

private:
	// Calculates the size of the background and formats the text objects.
	void format();
	
	sf::RectangleShape background;
	std::vector<sfe::RichText> tooltipText;
};