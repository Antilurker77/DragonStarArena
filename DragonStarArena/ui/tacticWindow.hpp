// ================================================================
//
// tacticWindow.hpp
// 
// Window where player can view and changed AI tactics.
//
// ================================================================

#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Window.hpp>
#include "button.hpp"
#include "dropdown.hpp"
#include "richText.hpp"
#include "textBox.hpp"
#include "tooltip.hpp"
#include "../entity/entity.hpp"

typedef std::shared_ptr<Actor> ActorPtr;

class TacticWindow {
public:
	TacticWindow();

	// Updates the tactic window.
	void Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick, bool draggingLeft, bool scrollUp, bool scrollDown);

	// Draws the tactic window.
	void Render(sf::RenderTarget& window);

	// Sets the player reference.
	void SetPlayerList(std::vector<ActorPtr>& list);

private:
	// ================================
	Tooltip tooltip;
	bool displayTooltip = false;

	sf::RectangleShape background;
	std::array<Button, 6> playerButtons;

	std::vector<ActorPtr> players{};
	size_t viewedPlayer = 0;
};