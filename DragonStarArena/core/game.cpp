// ================================================================
//
// game.cpp
//
// ================================================================

#include "game.hpp"

#include "random.hpp"
#include "settings.hpp"

// Time per update.
const sf::Time timePerUpdate = sf::seconds(1.f / 100.f);

// Game title.
const std::string gameTitle = "Dragon Star Arena v0.1.0";

Game::Game() :
	window(sf::VideoMode(settings.ScreenWidth, settings.ScreenHeight), gameTitle)
{
	window.setVerticalSyncEnabled(settings.EnableVSync);

	gameState = GameState::MainMenu;
}

void Game::Run() {
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;
	float timeRatio = 0.f;
	float secondsPerUpdate = timePerUpdate.asSeconds();

	while (window.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceUpdate += elapsedTime;
		timeRatio = 0.f;

		while (timeSinceUpdate > timePerUpdate) {
			timeSinceUpdate -= timePerUpdate;
			processInput();
			update(secondsPerUpdate);
		}

		timeRatio = timeSinceUpdate.asSeconds() / timePerUpdate.asSeconds();
		draw(timeRatio);
		fpsWindow.Update(elapsedTime);
	}
}

void Game::processInput() {
	switch (gameState) {
	case GameState::MainMenu:
		mainMenuScene.ReadInput(window);
		break;
	case GameState::DungeonMap:
		dungeonMapScene.ReadInput(window);
		break;
	case GameState::Battle:
		battleScene.ReadInput(window);
		break;
	default:
		break;
	}
}

void Game::update(float secondsPerUpdate) {
	switch (gameState) {
	case GameState::MainMenu:
		gameState = mainMenuScene.Update(secondsPerUpdate);

		if (gameState == GameState::DungeonMap) {
			dungeonMapScene.CreateParty();
			dungeonMapScene.GenerateEncounterNodes(Random::RandomSeed());
		}

		if (gameState == GameState::Battle) {
			battleScene.InitTestBattle();
		}

		break;
	case GameState::DungeonMap:
		gameState = dungeonMapScene.Update(secondsPerUpdate);

		if (gameState == GameState::Battle) {
			battleScene.InitBattle(dungeonMapScene.GetParty(), dungeonMapScene.GetChoosenEncounter());
		}

		break;
	case GameState::Battle:
		gameState = battleScene.Update(secondsPerUpdate);

		if (gameState == GameState::DungeonMap) {
			dungeonMapScene.CompleteNode();
			dungeonMapScene.AddGold(battleScene.GetGoldAwarded());
		}
		break;
	default:
		break;
	}
}

void Game::draw(float timeRatio) {
	window.clear();

	switch (gameState) {
	case GameState::MainMenu:
		mainMenuScene.Render(window, timeRatio);
		break;
	case GameState::DungeonMap:
		dungeonMapScene.Render(window, timeRatio);
		break;
	case GameState::Battle:
		battleScene.Render(window, timeRatio);
		break;
	default:
		break;
	}

	if (settings.DrawFPS) {
		fpsWindow.Render(window);
	}

	window.display();
}