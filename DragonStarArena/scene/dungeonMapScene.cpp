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
	window.draw(lines);
	
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

	encounterNodes.clear();
	connections.clear();

	int partyLevel = getAveragePartyLevel();
	size_t stages = Random::RandomSizeT(7, 9, mt);
	const size_t rows = 6;
	std::vector<std::array<bool, rows>> nodesToPlace;
	std::vector<LocationPair> connectionsToBuild;
	nodesToPlace.resize(stages + 1);

	for (size_t i = 0; i < nodesToPlace.size(); i++) {
		for (size_t j = 0; j < nodesToPlace[i].size(); j++) {
			nodesToPlace[i][j] = false;
		}
	}

	// 3 Paths
	size_t pathsToMake = 3;
	for (size_t i = 0; i < pathsToMake; i++) {
		size_t previousColumn = 0;
		for (size_t j = 0; j < stages; j++) {
			size_t column = 0;
			// First Node
			if (j == 0) {
				column = Random::RandomSizeT(0, rows - 1, mt);
				previousColumn = column;
			}
			else {
				if (previousColumn == 0) {
					column = Random::RandomSizeT(0, 1, mt);
				}
				else if (previousColumn == rows - 1) {
					column = Random::RandomSizeT(rows - 2, rows - 1, mt);
				}
				else {
					column = Random::RandomSizeT(previousColumn - 1, previousColumn + 1, mt);
				}
				previousColumn = column;
			}
			nodesToPlace[j][column] = true;
		}
	}

	for (size_t i = 0; i < nodesToPlace.size(); i++) {
		for (size_t j = 0; j < nodesToPlace[i].size(); j++) {
			if (nodesToPlace[i][j]) {
				sf::Vector2f pos(0.f, 0.f);
				pos.x = (settings.ScreenWidthF / (stages + 1)) * (i + 1);
				pos.y = (settings.ScreenHeightF / (rows + 2)) * (j + 2);

				EncounterNode node(pos, i, j, partyLevel);
				node.VaryPosition(8.f);
				encounterNodes.push_back(node);

				// Connections
				if (i < stages - 1) {
					if (j == 0) {
						if (nodesToPlace[i + 1][0]) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j} });
						}
						if (nodesToPlace[i + 1][1] && (!nodesToPlace[i][1] || !nodesToPlace[i + 1][0])) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j + 1} });
						}
					}
					else if (j == rows - 1) {
						if (nodesToPlace[i + 1][rows - 2] && (!nodesToPlace[i][rows - 2] || !nodesToPlace[i + 1][rows - 1])) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j - 1} });
						}
						if (nodesToPlace[i + 1][rows - 1]) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j} });
						}
					}
					else {
						if (nodesToPlace[i + 1][j - 1] && (!nodesToPlace[i][j - 1] || !nodesToPlace[i + 1][j])) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j - 1} });
						}
						if (nodesToPlace[i + 1][j]) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j} });
						}
						if (nodesToPlace[i + 1][j + 1] && (!nodesToPlace[i][j + 1] || !nodesToPlace[i + 1][j])) {
							connectionsToBuild.push_back({ {i, j}, {i + 1, j + 1} });
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < connectionsToBuild.size(); i++) {
		size_t first = 0;
		size_t second = 0;

		for (size_t j = 0; j < encounterNodes.size(); j++) {
			if (encounterNodes[j].GetLocation() == connectionsToBuild[i].first) {
				first = j;
				break;
			}
		}
		for (size_t j = 0; j < encounterNodes.size(); j++) {
			if (encounterNodes[j].GetLocation() == connectionsToBuild[i].second) {
				second = j;
				break;
			}
		}
		connections.push_back({ first, second });
	}

	// build connection lines
	buildLineVertexArray();
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

void DungeonMapScene::buildLineVertexArray() {
	size_t j = 0;
	lines.clear();
	lines.resize(connections.size() * 2);
	lines.setPrimitiveType(sf::Lines);

	for (auto& conn : connections) {
		sf::Vertex* line = &lines[j * 2];

		sf::Vector2f start = encounterNodes[conn.first].GetSpritePosition();
		sf::Vector2f end = encounterNodes[conn.second].GetSpritePosition();

		start.x += 32.f;
		start.y += 32.f;
		end.x += 32.f;
		end.y += 32.f;

		line[0].position = start;
		line[0].color = sf::Color(255, 255, 255, 255);

		line[1].position = end;
		line[1].color = sf::Color(255, 255, 255, 255);

		j++;
	}
}