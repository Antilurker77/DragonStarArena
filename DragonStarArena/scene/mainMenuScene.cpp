// ================================================================
//
// mainMenuScene.cpp
//
// ================================================================

#include "mainMenuScene.hpp"

#include "../core/assetManager.hpp"
#include "../core/gameState.hpp"
#include "../core/settings.hpp"

MainMenuScene::MainMenuScene() {
	titleText.setString("Dragon Star");
	titleText.setCharacterSize(64);
	titleText.setFont(*assetManager.LoadFont(settings.Font));

	subtitleText.setString("A R E N A");
	subtitleText.setCharacterSize(32);
	subtitleText.setFont(*assetManager.LoadFont(settings.Font));

	newGameButton.SetString("New Game");
	testBattleButton.SetString("Test Battle");

	SetPositions();
}

void MainMenuScene::ReadInput(sf::RenderWindow& window) {
	sf::Event ev;
	sf::Mouse mouse;

	leftClick = false;
	rightClick = false;
	mousePos = mouse.getPosition(window);

	while (window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				leftClick = true;
			}
			else if (ev.mouseButton.button == sf::Mouse::Right) {
				rightClick = true;
			}
			break;
		default:
			break;
		}
	}

}

GameState MainMenuScene::Update(float secondsPerUpdate) {
	GameState gs = GameState::MainMenu;

	if (newGameButton.Update(secondsPerUpdate, mousePos)) {
		if (leftClick) {
			gs = GameState::DungeonMap;
		}
	}
	if (testBattleButton.Update(secondsPerUpdate, mousePos)) {
		if (leftClick) {
			gs = GameState::Battle;
		}
	}

	return gs;
}

void MainMenuScene::Render(sf::RenderTarget& window, float timeRatio) {
	window.draw(titleText);
	window.draw(subtitleText);
	newGameButton.Render(window);
	testBattleButton.Render(window);
}

void MainMenuScene::SetPositions() {
	auto titleTextSize = titleText.getLocalBounds();
	auto subtitleTextSize = subtitleText.getLocalBounds();
	sf::Vector2f pos;

	pos.x = std::roundf(settings.ScreenWidthF / 2.f - titleTextSize.width / 2.f);
	pos.y = std::roundf(settings.ScreenHeightF / 2.f - 200.f);
	titleText.setPosition(pos);

	pos.x = std::roundf(settings.ScreenWidthF / 2.f - subtitleTextSize.width / 2.f);
	pos.y = std::roundf(pos.y + 12.f + titleTextSize.height);
	subtitleText.setPosition(pos);

	pos.x = settings.ScreenWidthF * 0.5f;
	pos.y = settings.ScreenHeightF * 0.7f;
	newGameButton.SetPosition(pos);

	pos.x = settings.ScreenWidthF * 0.5f;
	pos.y = settings.ScreenHeightF * 0.75f;
	testBattleButton.SetPosition(pos);
}