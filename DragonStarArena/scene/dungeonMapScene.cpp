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
#include "../data/id/encounterType.hpp"

DungeonMapScene::DungeonMapScene() {
	nodeTexture = assetManager.LoadTexture("gfx/node/nodes.png");

	sf::Vector2f size = resourceWindow.GetSize();
	resourceWindow.SetPosition(settings.ScreenWidthF - size.x - 2.f, 2.f);

	inventoryButton.SetString("Inventory", 24u);
	inventoryButton.SetPosition(settings.ScreenWidthF * 0.10f, settings.ScreenHeightF * 0.95f);

	// test
	inventory.push_back(Item(40));
	inventory.push_back(Item(40));
	inventory.push_back(Item(3));
	inventory.push_back(Item(40));
	inventory.push_back(Item(11));
	inventory.push_back(Item(30));
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
	displayTooltip = false;

	choosenEncounter = nullptr;

	if (!windowOpen) {
		for (size_t i = 0; i < nodeBoxes.size(); i++) {
			if (nodeBoxes[i].getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
				tooltip.SetTooltip(&encounterNodes[i]);
				auto tooltipSize = tooltip.GetSize();
				tooltip.SetPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) - tooltipSize.y);
				displayTooltip = true;
				if (leftClick) {
					if (!hasStarted && encounterNodes[i].GetLocation().first == 0) {
						choosenEncounter = &encounterNodes[i];
						hasStarted = true;
						currentLocation = encounterNodes[i].GetLocation();
					}
					else if (std::find(availableNodes.begin(), availableNodes.end(), i) != availableNodes.end()) {
						choosenEncounter = &encounterNodes[i];
						hasStarted = true;
						currentLocation = encounterNodes[i].GetLocation();
					}
				}
			}
		}
	}

	if (choosenEncounter != nullptr) {
		if (choosenEncounter->GetEncounterData() != nullptr) {
			gs = GameState::Battle;
		}
		else {
			CompleteNode();
		}
	}

	if (inventoryButton.Update(secondsPerUpdate, mousePos)) {
		if (leftClick) {
			displayInventory = !displayInventory;
			windowOpen = displayInventory;
			if (displayInventory) {
				inventoryList.SetList(inventory);
				auto ilSize = inventoryList.GetSize();
				inventoryList.SetPosition(settings.ScreenWidthF / 2.f - ilSize.x / 2.f, settings.ScreenHeightF / 2.f - ilSize.y / 2.f);
			}
		}
	}

	if (displayInventory) {
		inventoryList.Update(secondsPerUpdate, mousePos);
	}

	return gs;
}

void DungeonMapScene::Render(sf::RenderTarget& window, float timeRatio) {
	window.draw(lines);
	window.draw(nodes, nodeTexture);

	resourceWindow.Render(window);

	inventoryButton.Render(window);

	if (displayTooltip && !windowOpen) {
		tooltip.Render(window);
	}

	if (displayInventory) {
		inventoryList.Render(window);
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
	const float variance = 24.f;
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
				pos.x = (settings.ScreenWidthF / (stages + 2)) * (i + 1);
				pos.y = (settings.ScreenHeightF / (rows + 2)) * (j + 2);

				EncounterNode node(pos, i, j, partyLevel, mt);
				node.VaryPosition(variance);
				encounterNodes.push_back(node);

				sf::RectangleShape hitBox;
				hitBox.setSize(sf::Vector2f(48.f, 48.f));
				hitBox.setPosition(node.GetSpritePosition().x + 12.f, node.GetSpritePosition().y + 12.f);
				nodeBoxes.push_back(hitBox);

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
				if (i == stages - 1) {
					connectionsToBuild.push_back({ {i, j}, {i + 1, (rows / 2) + (rows % 2)} });
				}
			}
		}
	}

	// Boss Node
	sf::Vector2f pos(0.f, 0.f);
	pos.x = (settings.ScreenWidthF / (stages + 2)) * (stages + 1);
	pos.y = (settings.ScreenHeightF / (rows + 2)) * ((rows / 2) + 2 + (rows % 2));

	EncounterNode node(pos, stages, (rows / 2) + (rows % 2), partyLevel, mt);
	node.VaryPosition(variance);
	node.SetBossNode(partyLevel, mt);
	encounterNodes.push_back(node);

	sf::RectangleShape hitBox;
	hitBox.setSize(sf::Vector2f(48.f, 48.f));
	hitBox.setPosition(node.GetSpritePosition().x + 12.f, node.GetSpritePosition().y + 12.f);
	nodeBoxes.push_back(hitBox);


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

	BuildNodeVertexArray();

	// build connection lines
	buildLineVertexArray();
}

void DungeonMapScene::GenerateEncounterNodes(uint64_t seed) {
	masterSeed = seed;
	GenerateEncounterNodes();
}

void DungeonMapScene::BuildNodeVertexArray() {
	size_t i = 0;
	nodes.clear();
	nodes.resize(encounterNodes.size() * 8);
	nodes.setPrimitiveType(sf::Quads);

	for (auto& node : encounterNodes) {
		// Inner Node
		sf::Vertex* quad = &nodes[i * 8];

		sf::Vector2f pos = node.GetSpritePosition();

		// quad cords
		// 0  1
		// 3  2
		quad[0].position = pos;
		quad[1].position = sf::Vector2f{ pos.x + 64.f, pos.y };
		quad[2].position = sf::Vector2f{ pos.x + 64.f, pos.y + 64.f };
		quad[3].position = sf::Vector2f{ pos.x, pos.y + 64.f };

		sf::Vector2f texPos{};

		switch (node.GetEncounterType()) {
		case EncounterType::Battle:
			texPos.x = 0.f;
			texPos.y = 64.f;
			break;
		case EncounterType::Treasure:
			texPos.x = 64.f;
			texPos.y = 64.f;
			break;
		case EncounterType::SpecialShop:
			texPos.x = 128.f;
			texPos.y = 64.f;
			break;
		case EncounterType::UniqueBattle:
			texPos.x = 192.f;
			texPos.y = 64.f;
			break;
		default:
			break;
		}

		quad[0].texCoords = sf::Vector2f{ texPos.x, texPos.y };
		quad[1].texCoords = sf::Vector2f{ texPos.x + 64.f, texPos.y };
		quad[2].texCoords = sf::Vector2f{ texPos.x + 64.f, texPos.y + 64.f };
		quad[3].texCoords = sf::Vector2f{ texPos.x, texPos.y + 64.f };

		// Border
		quad = &nodes[i * 8 + 4];

		quad[0].position = pos;
		quad[1].position = sf::Vector2f{ pos.x + 64.f, pos.y };
		quad[2].position = sf::Vector2f{ pos.x + 64.f, pos.y + 64.f };
		quad[3].position = sf::Vector2f{ pos.x, pos.y + 64.f };

		if (hasStarted == false && node.GetLocation().first == 0) {
			texPos.x = 128.f;
			texPos.y = 0.f;
		}
		else if (hasStarted && node.GetLocation().first == currentLocation.first && node.GetLocation().second == currentLocation.second) {
			texPos.x = 64.f;
			texPos.y = 0.f;
		}
		else if (std::find(availableNodes.begin(), availableNodes.end(), i) != availableNodes.end()) {
			texPos.x = 128.f;
			texPos.y = 0.f;
		}
		else {
			texPos.x = 0.f;
			texPos.y = 0.f;
		}

		quad[0].texCoords = sf::Vector2f{ texPos.x, texPos.y };
		quad[1].texCoords = sf::Vector2f{ texPos.x + 64.f, texPos.y };
		quad[2].texCoords = sf::Vector2f{ texPos.x + 64.f, texPos.y + 64.f };
		quad[3].texCoords = sf::Vector2f{ texPos.x, texPos.y + 64.f };

		i++;
	}
}

void DungeonMapScene::CompleteNode() {
	availableNodes.clear();

	size_t currentNodeIndex = 0;

	for (size_t i = 0; i < encounterNodes.size(); i++) {
		if (encounterNodes[i].GetLocation() == currentLocation) {
			currentNodeIndex = i;
			break;
		}
	}

	for (size_t i = 0; i < connections.size(); i++) {
		if (connections[i].first == currentNodeIndex) {
			availableNodes.push_back(connections[i].second);
		}
	}

	BuildNodeVertexArray();
}

void DungeonMapScene::AddGold(int64_t amount) {
	gold += std::max(0ll, amount);
	resourceWindow.SetGold(gold);

	sf::Vector2f size = resourceWindow.GetSize();
	resourceWindow.SetPosition(settings.ScreenWidthF - size.x - 2.f, 2.f);
}

std::vector<ActorPtr> DungeonMapScene::GetParty() {
	return party;
}

EncounterNode* DungeonMapScene::GetChoosenEncounter() {
	return choosenEncounter;
}

int DungeonMapScene::getAveragePartyLevel() {
	int result = 0;
	for (size_t i = 0; i < party.size(); i++) {
		result += party[i]->GetLevel();
	}
	return result / static_cast<int>(party.size());
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