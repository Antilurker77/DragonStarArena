// ================================================================
//
// tacticWindow.cpp
//
// ================================================================

#include "tacticWindow.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../entity/actor.hpp"

TacticWindow::TacticWindow() {
	background.setSize(sf::Vector2f(800.f, 450.f));
	background.setFillColor(sf::Color(0, 0, 0, 191));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));
	background.setPosition(settings.ScreenWidthF / 2.f - 400.f, settings.ScreenHeightF / 2.f - 225.f);
}

void TacticWindow::Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick, bool draggingLeft, bool scrollUp, bool scrollDown) {
	sf::Vector2f mousePosF{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	displayTooltip = false;

	for (size_t i = 0; i < playerButtons.size(); i++) {
		if (playerButtons[i].Update(secondsPerUpdate, mousePos)) {
			if (leftClick) {
				viewedPlayer = i;
			}
		}
	}
}

void TacticWindow::Render(sf::RenderTarget& window) {
	window.draw(background);

	for (size_t i = 0; i < playerButtons.size(); i++) {
		playerButtons[i].Render(window);
	}

	if (displayTooltip) {
		tooltip.Render(window);
	}
}

void TacticWindow::SetPlayerList(std::vector<ActorPtr>& list) {
	players = list;

	auto pos = background.getPosition();
	pos.y -= 30.f;

	for (size_t i = 0; i < players.size(); i++) {
		playerButtons[i].SetString(players.at(i)->GetName(), 16u);
		if (i > 0) {
			auto buttonSize = playerButtons[i - 1].GetSize();
			pos.x += 4.f + buttonSize.x + playerButtons[i].GetSize().x / 2.f;
		}
		playerButtons[i].SetPosition(pos);
	}
}