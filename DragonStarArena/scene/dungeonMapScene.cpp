// ================================================================
//
// dungeonMapScene.cpp
//
// ================================================================

#include "dungeonMapScene.hpp"

#include "../core/gameState.hpp"
#include "../core/settings.hpp"

DungeonMapScene::DungeonMapScene() {

}

void DungeonMapScene::ReadInput(sf::RenderWindow& window) {
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

GameState DungeonMapScene::Update(float secondsPerUpdate) {
	GameState gs = GameState::DungeonMap;

	return gs;
}

void DungeonMapScene::Render(sf::RenderTarget& window, float timeRatio) {

}