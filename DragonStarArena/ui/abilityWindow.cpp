// ================================================================
//
// abilityWindow.cpp
//
// ================================================================

#include "abilityWindow.hpp"

#include "dataString.hpp"
#include "../core/assetManager.hpp"
#include "../core/settings.hpp"
#include "../data/item.hpp"
#include "../data/id/itemType.hpp"
#include "../entity/actor.hpp"

AbilityWindow::AbilityWindow() {
	background.setSize(sf::Vector2f(800.f, 450.f));
	background.setFillColor(sf::Color(0, 0, 0, 191));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));
	background.setPosition(settings.ScreenWidthF / 2.f - 400.f, settings.ScreenHeightF / 2.f - 225.f);

	auto bgPos = background.getPosition();
	bgPos.x += 160.f;
	bgPos.y += 16.f;

	for (size_t i = 0; i < equippedAbilitiesBG.size(); i++) {
		equippedAbilitiesBG[i].setSize(sf::Vector2f(32.f, 32.f));
		equippedAbilitiesBG[i].setFillColor(sf::Color(0, 0, 0, 255));
		equippedAbilitiesBG[i].setOutlineThickness(1.f);
		equippedAbilitiesBG[i].setOutlineColor(sf::Color(255, 255, 255, 255));

		equippedAbilitiesBG[i].setPosition(bgPos);
		equippedAbilitiesIcons[i].SetSpriteCount(1);
		equippedAbilitiesIcons[i].SetScale(2.f);
		equippedAbilitiesIcons[i].Move(bgPos);

		//drawEquippedAbilitiesIcons[i] = false;

		bgPos.x += 40.f;

		if (i == 0) {
			bgPos.x += 40.f;
		}
	}

	bgPos = background.getPosition();
	bgPos.x += 10.f;
	bgPos.y += 60.f;

	knownAbilitiesBG.setSize(sf::Vector2f(380.f, 380.f));
	knownAbilitiesBG.setFillColor(sf::Color(0, 0, 0, 255));
	knownAbilitiesBG.setOutlineThickness(1.f);
	knownAbilitiesBG.setOutlineColor(sf::Color(255, 255, 255, 255));
	knownAbilitiesBG.setPosition(bgPos);
}

void AbilityWindow::Update(float secondsPerUpdate, sf::Vector2i mousePos, bool leftClick, bool rightClick, bool draggingLeft) {
	sf::Vector2f mousePosF{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	displayTooltip = false;

	if (dragging) {
		entityOnCursor.Move(sf::Vector2f(mousePosF.x - 16.f, mousePosF.y - 16.f));
	}

	for (size_t i = 0; i < playerButtons.size(); i++) {
		if (playerButtons[i].Update(secondsPerUpdate, mousePos)) {
			if (leftClick) {
				viewedPlayer = i;
				setAbilities();
			}
		}
	}

	for (size_t i = 0; i < equippedAbilitiesBG.size(); i++) {
		if (drawEquippedAbilitiesIcons[i] && equippedAbilitiesBG[i].getGlobalBounds().contains(mousePosF)) {
			displayTooltip = true;
			tooltip.SetTooltip(&equippedAbilities->at(i), players.at(viewedPlayer).get());
			auto size = tooltip.GetSize();
			tooltip.SetPosition(mousePosF.x, mousePosF.y - size.y);
			if (rightClick) {

			}
		}
	}
}

void AbilityWindow::Render(sf::RenderTarget& window) {
	window.draw(background);

	for (size_t i = 0; i < playerButtons.size(); i++) {
		playerButtons[i].Render(window);
	}

	for (size_t i = 0; i < equippedAbilitiesBG.size(); i++) {
		window.draw(equippedAbilitiesBG[i]);
	}

	for (size_t i = 0; i < equippedAbilitiesIcons.size(); i++) {
		if (drawEquippedAbilitiesIcons[i]) {
			equippedAbilitiesIcons[i].Render(window, 0.f);
		}
	}

	window.draw(knownAbilitiesBG);

	if (displayTooltip) {
		tooltip.Render(window);
	}
}

void AbilityWindow::SetPlayerList(std::vector<ActorPtr>& list) {
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

	setAbilities();
}

void AbilityWindow::setAbilities() {
	equippedAbilities = players.at(viewedPlayer)->GetAbilities();

	for (size_t i = 0; i < equippedAbilitiesBG.size(); i++) {
		if (equippedAbilities->at(i).GetAbilityID() != 0) {
			equippedAbilitiesIcons[i].SetTexture(equippedAbilities->at(i).GetIcon());
			drawEquippedAbilitiesIcons[i] = true;
		}
		else {
			drawEquippedAbilitiesIcons[i] = false;
		}
	}
}