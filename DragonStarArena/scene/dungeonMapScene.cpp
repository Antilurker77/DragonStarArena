// ================================================================
//
// dungeonMapScene.cpp
//
// ================================================================

#include "dungeonMapScene.hpp"

#include <vector>
#include "../core/gameState.hpp"
#include "../core/random.hpp"
#include "../core/settings.hpp"
#include "../data/id/battlePosition.hpp"

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
	for (size_t i = 0; i < encounterNodes.size(); i++) {
		encounterNodes[i].Render(window, timeRatio);
	}
}

void DungeonMapScene::CreateParty() {
	party.push_back(std::make_shared<Player>("Human", 1, BattlePosition::FrontCenter));
	party.push_back(std::make_shared<Player>("Elf", 2, BattlePosition::FrontLeft));
	party.push_back(std::make_shared<Player>("Stone Dwarf", 4, BattlePosition::FrontRight));
	party.push_back(std::make_shared<Player>("Shadow Elf", 3, BattlePosition::BackLeft));
	party.push_back(std::make_shared<Player>("Draconian", 5, BattlePosition::BackCenter));
	party.push_back(std::make_shared<Player>("Vulpine", 6, BattlePosition::BackRight));
}

void DungeonMapScene::GenerateEncounterNodes() {
	std::mt19937_64 mt(masterSeed);

	int partyLevel = getAveragePartyLevel();
	size_t stages = Random::RandomSizeT(7, 9, mt) + 1;
	std::vector<size_t> encountersPerStage(stages);

	for (size_t i = 0; i < encountersPerStage.size(); i++) {
		encountersPerStage[i] = Random::RandomSizeT(1, 3, mt);

		for (size_t j = 0; j < encountersPerStage[i]; j++) {
			sf::Vector2f pos(0.f, 0.f);
			pos.x = (settings.ScreenWidthF / stages) * i;
			pos.y = (settings.ScreenHeightF / 7.f) * (j + 3);

			EncounterNode node(pos, partyLevel);
			encounterNodes.push_back(node);
		}
	}
}

void DungeonMapScene::GenerateEncounterNodes(uint64_t seed) {
	masterSeed = seed;
	GenerateEncounterNodes();
}

int DungeonMapScene::getAveragePartyLevel() {
	int result = 0;
	for (size_t i = 0; i < party.size(); i++) {
		result += party[i]->GetLevel();
	}
	return result / 6;
}