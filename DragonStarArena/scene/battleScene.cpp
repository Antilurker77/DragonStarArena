// ================================================================
//
// battleScene.cpp
//
// ================================================================

#include "battleScene.hpp"

#include <iostream>
#include "../core/assetManager.hpp"
#include "../core/gameState.hpp"
#include "../core/random.hpp"
#include "../core/settings.hpp"
#include "../data/enconterData.hpp"
#include "../data/id/battlePosition.hpp"
#include "../entity/encounterNode.hpp"
#include "../entity/monster.hpp"
#include "../entity/player.hpp"

BattleScene::BattleScene() {
	messageLogBG.setFillColor(sf::Color(0, 0, 0, 255));
	messageLogBG.setOutlineColor(sf::Color(255, 255, 255, 255));
	messageLogBG.setOutlineThickness(1.f);
	
	float fFontSize = static_cast<float>(messageLogFontSize);
	float fMaxMessages = static_cast<float>(messagesDisplayed);

	messageLogBG.setSize(sf::Vector2f(4.f + fFontSize * 50.f, 4.f + fMaxMessages * (fFontSize + 2.f)));
	messageLogBG.setPosition(settings.ScreenWidthF / 2.f - messageLogBG.getSize().x / 2.f, settings.ScreenHeightF - messageLogBG.getSize().y - 4.f);

	const float actorBoxSize = 64.f;
	sf::Vector2f actorBoxPos{ settings.ScreenWidthF / 2.f - (actorBoxSize * 2.5f), 7.f };
	for (size_t i = 0; i < actorBoxes.size(); i++) {
		actorBoxes[i].setSize(sf::Vector2f(actorBoxSize, actorBoxSize));
		actorBoxes[i].setFillColor(sf::Color(0, 0, 0, 255));
		actorBoxes[i].setOutlineColor(sf::Color(255, 255, 255, 255));
		actorBoxes[i].setOutlineThickness(1.f);

		sf::Vector2f pos(actorBoxPos);
		pos.x = pos.x + (i / 5) * (actorBoxSize + 4.f);
		if (i >= 10) {
			pos.x += actorBoxSize + 8.f;
		}
		pos.y = pos.y + (i % 5) * (actorBoxSize + 4.f);

		actorBoxes[i].setPosition(pos);
	}

	continueButton.SetString("Continue", 32u);
	continueButton.SetPosition(settings.ScreenWidthF / 2.f, actorBoxSize * 2.f + 7.f);
}

void BattleScene::InitTestBattle() {
	timePassed = 0;
	victory = false;
	defeat = false;
	
	actors.push_back(std::make_shared<Player>("Human", 1, BattlePosition::FrontCenter));
	actors.push_back(std::make_shared<Player>("Elf", 2, BattlePosition::FrontLeft));
	actors.push_back(std::make_shared<Player>("Stone Dwarf", 4, BattlePosition::FrontRight));
	actors.push_back(std::make_shared<Player>("Shadow Elf", 3, BattlePosition::BackLeft));
	actors.push_back(std::make_shared<Player>("Draconian", 5, BattlePosition::BackCenter));
	actors.push_back(std::make_shared<Player>("Vulpine", 6, BattlePosition::BackRight));
	actors.push_back(std::make_shared<Monster>(7, BattlePosition::FrontCenter)); // Fire Ant
	actors.push_back(std::make_shared<Monster>(5, BattlePosition::FrontLeft)); // Giant Rat
	actors.push_back(std::make_shared<Monster>(6, BattlePosition::FrontRight)); // Black Snake

	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->SetIndex(i);
	}

	// randomize exhaustion
	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->Exhaust(Random::RandomInt(1, 99));
	}

	playerText.setFont(*assetManager.LoadFont(settings.Font));
	playerText.setCharacterSize(16u);
	playerText.setPosition(10.f, 80.f);

	enemyText.setFont(*assetManager.LoadFont(settings.Font));
	enemyText.setCharacterSize(16u);
	enemyText.setPosition(1080.f, 80.f);

	setActorSpritePositions();

	AddMessage("Let the battle begin!");
}

void BattleScene::InitBattle(std::vector<ActorPtr> party, EncounterNode* encounterNode) {
	timePassed = 0;
	victory = false;
	defeat = false;
	actors.clear();
	actors.reserve(16);

	actors.insert(actors.end(), party.begin(), party.end());
	EncounterData* ed = encounterNode->GetEncounterData();

	if (ed->FrontFarLeftMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->FrontFarLeftMonster, BattlePosition::FrontFarLeft));
	}
	if (ed->FrontLeftMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->FrontLeftMonster, BattlePosition::FrontLeft));
	}
	if (ed->FrontCenterMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->FrontCenterMonster, BattlePosition::FrontCenter));
	}
	if (ed->FrontRightMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->FrontRightMonster, BattlePosition::FrontRight));
	}
	if (ed->FrontFarRightMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->FrontFarRightMonster, BattlePosition::FrontFarRight));
	}
	if (ed->BackFarLeftMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->BackFarLeftMonster, BattlePosition::FrontFarLeft));
	}
	if (ed->BackLeftMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->BackLeftMonster, BattlePosition::FrontLeft));
	}
	if (ed->BackCenterMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->BackCenterMonster, BattlePosition::FrontCenter));
	}
	if (ed->BackRightMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->BackRightMonster, BattlePosition::FrontRight));
	}
	if (ed->BackFarRightMonster != 0) {
		actors.push_back(std::make_shared<Monster>(ed->BackFarRightMonster, BattlePosition::FrontFarRight));
	}

	actors.shrink_to_fit();

	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->SetIndex(i);
	}

	// randomize exhaustion
	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->Exhaust(Random::RandomInt(1, 99));
	}

	playerText.setFont(*assetManager.LoadFont(settings.Font));
	playerText.setCharacterSize(16u);
	playerText.setPosition(10.f, 80.f);

	enemyText.setFont(*assetManager.LoadFont(settings.Font));
	enemyText.setCharacterSize(16u);
	enemyText.setPosition(1080.f, 80.f);

	setActorSpritePositions();

	AddMessage("Let the battle begin!");
}

void BattleScene::ReadInput(sf::RenderWindow& window) {
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

GameState BattleScene::Update(float secondsPerUpdate) {
	GameState gs = GameState::Battle;

	// temp debug
	std::string playerString;
	std::string enemyString;

	for (size_t i = 0; i < actors.size(); i++) {
		std::string s1 = actors[i]->GetName();
		std::string s2 = "HP: " + std::to_string(actors[i]->GetCurrentHP()) + "/" + std::to_string(actors[i]->GetMaxHP());
		std::string s3 = "MP: " + std::to_string(actors[i]->GetCurrentMP()) + "    SP: " + std::to_string(actors[i]->GetCurrentSP());

		if (actors[i]->IsPlayer()) {
			playerString += s1 + "\n" + s2 + "\n" + s3 + "\n\n";
		}
		else {
			enemyString += s1 + "\n" + s2 + "\n" + s3 + "\n\n";
		}
	}

	playerText.setString(playerString);
	enemyText.setString(enemyString);

	// Battle System
	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->Update(secondsPerUpdate);

		// remove auras from dead actors
		if (!actors[i]->IsAlive()) {
			actors[i]->ClearAuras();
		}
	}

	// check if there's an actor ready to go
	if (!victory && !defeat) {
		for (size_t i = 0; i < actors.size(); i++) {
			if (actors[i]->IsReady()) {
				auto ai = actors[i]->CalcTactics(actors);
				if (ai.first == 0) {
					actors[i]->Exhaust(100);
					AddMessage(actors[i]->GetName() + " failed to find a tactic.");
				}
				else {
					actors[i]->UseAbility(ai.second, ai.first, this);
				}
			}
		}
	}

	// no actors ready, decrement exhaustion
	for (size_t i = 0; i < actors.size(); i++) {
		actors[i]->DecrementExhaustion(this);
	}

	// check for victory or defeat
	if (!victory && !defeat) {
		// victory
		victory = true;
		for (size_t i = 0; i < actors.size(); i++) {
			if (!actors[i]->IsPlayer() && actors[i]->IsAlive()) {
				victory = false;
				break;
			}
		}
		// defeat
		if (!victory) {
			defeat = true;
			for (size_t i = 0; i < actors.size(); i++) {
				if (actors[i]->IsPlayer() && actors[i]->IsAlive()) {
					defeat = false;
					break;
				}
			}
		}

		if (victory) {
			AddMessage("Your party is victorious!");
		}
		else if (defeat) {
			AddMessage("Your party has been slain.");
		}
	}

	timePassed++;

	// UI
	if (victory || defeat) {
		if (continueButton.Update(secondsPerUpdate, mousePos) && leftClick) {
			if (victory) {
				gs = GameState::DungeonMap;

				for (size_t i = 0; i < actors.size(); i++) {
					if (actors[i]->IsPlayer()) {
						actors[i]->PostBattleRecovery();
					}
				}
			}
			else {
				gs = GameState::MainMenu;
			}
		}
	}

	return gs;
}

void BattleScene::Render(sf::RenderTarget& window, float timeRatio) {
	// debug
	window.draw(playerText);
	window.draw(enemyText);

	// UI
	for (size_t i = 0; i < actorBoxes.size(); i++) {
		window.draw(actorBoxes[i]);
	}

	for (size_t i = 0; i < actors.size(); i++) {
		if (actors[i]->IsAlive()) {
			actors[i]->Render(window, timeRatio);
		}
	}

	window.draw(messageLogBG);

	size_t last = 0;
	if (messagesDisplayed < messageLog.size()) {
		last = messageLog.size() - messagesDisplayed;
	}
	for (size_t i = last; i < messageLog.size(); i++) {
		window.draw(messageLog[i]);
	}

	if (victory || defeat) {
		continueButton.Render(window);
	}
}

Actor* BattleScene::GetActorAtPosition(BattlePosition battlePosition, bool isPlayer) {
	for (size_t i = 0; i < actors.size(); i++) {
		if (actors[i]->IsPlayer() == isPlayer && actors[i]->GetBattlePosition() == battlePosition) {
			return actors[i].get();
		}
	}

	return nullptr;
}

void BattleScene::AddMessage(std::string message) {
	message = "[" + std::to_string(timePassed) + "] " + message;
	
	sfe::RichText rt;
	rt.setFont(*assetManager.LoadFont(settings.Font));
	rt.setString(message);
	rt.setCharacterSize(messageLogFontSize);
	messageLog.push_back(rt);

	auto pos = messageLogBG.getPosition();

	// Newest messages drawn on the bottom.
	size_t last = 0;
	if (messagesDisplayed < messageLog.size()) {
		last = messageLog.size() - messagesDisplayed;
	}
	for (size_t i = last; i < messageLog.size(); i++) {
		float rowsFromTop = static_cast<float>(i) - static_cast<float>(messageLog.size()) + static_cast<float>(messagesDisplayed);
		float size = static_cast<float>(messageLogFontSize) + 2.f;
		messageLog[i].setPosition(pos.x + 2.f, pos.y + 1.f + (rowsFromTop * size));
	}
}

void BattleScene::ClearMessageLog() {
	messageLog.clear();
}

void BattleScene::setActorSpritePositions() {
	//  0   5      10  15
	//  1   6      11  16
	//  2   7      12  17
	//  3   8      13  18
	//  4   9      14  19
	for (size_t i = 0; i < actors.size(); i++) {
		sf::Vector2f pos{};
		if (actors[i]->IsPlayer()) {
			switch (actors[i]->GetBattlePosition()) {
			case BattlePosition::FrontFarLeft:
				pos = actorBoxes[5].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontLeft:
				pos = actorBoxes[6].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontCenter:
				pos = actorBoxes[7].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontRight:
				pos = actorBoxes[8].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontFarRight:
				pos = actorBoxes[9].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackFarLeft:
				pos = actorBoxes[0].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackLeft:
				pos = actorBoxes[1].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackCenter:
				pos = actorBoxes[2].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackRight:
				pos = actorBoxes[3].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackFarRight:
				pos = actorBoxes[4].getPosition();
				actors[i]->Move(pos);
				break;
			default:
				break;
			}
		}
		else {
			switch (actors[i]->GetBattlePosition()) {
			case BattlePosition::FrontFarLeft:
				pos = actorBoxes[10].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontLeft:
				pos = actorBoxes[11].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontCenter:
				pos = actorBoxes[12].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontRight:
				pos = actorBoxes[13].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::FrontFarRight:
				pos = actorBoxes[14].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackFarLeft:
				pos = actorBoxes[15].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackLeft:
				pos = actorBoxes[16].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackCenter:
				pos = actorBoxes[17].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackRight:
				pos = actorBoxes[18].getPosition();
				actors[i]->Move(pos);
				break;
			case BattlePosition::BackFarRight:
				pos = actorBoxes[19].getPosition();
				actors[i]->Move(pos);
				break;
			default:
				break;
			}
		}
	}
}