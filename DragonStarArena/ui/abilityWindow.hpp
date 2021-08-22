// ================================================================
//
// abilityWindow.hpp
// 
// Window where player can equip and view abilities.
//
// ================================================================

#pragma once

class Ability;
class Actor;

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

class AbilityWindow {
public:
	AbilityWindow();

	// Updates the ability window.
	void Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick, bool draggingLeft);

	// Draws the ability window.
	void Render(sf::RenderTarget& window);

	// Sets the player reference.
	void SetPlayerList(std::vector<ActorPtr>& list);

private:
	// Sets the abilities.
	void setAbilities();

	// ================================
	Tooltip tooltip;
	bool displayTooltip = false;

	sf::RectangleShape background;
	std::array<Button, 6> playerButtons;

	std::vector<ActorPtr> players{};
	size_t viewedPlayer = 0;

	std::array<sf::RectangleShape, 11> equippedAbilitiesBG;
	std::array<Entity, 11> equippedAbilitiesIcons;
	std::array<bool, 11> drawEquippedAbilitiesIcons{ false, false, false, false, false, false, false, false, false, false, false };
	std::vector<Ability>* equippedAbilities = nullptr;

	sf::RectangleShape knownAbilitiesBG;

	bool dragging = false;
	float holdingDownTime = 0.f;
	Entity entityOnCursor;
	size_t indexOnCursor = 0;
};