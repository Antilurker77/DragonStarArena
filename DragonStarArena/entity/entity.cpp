// ================================================================
//
// entity.cpp
//
// ================================================================

#include "entity.hpp"

#include "../core/settings.hpp"

Entity::Entity() {

}

Entity::Entity(sf::Vector2f spawnPos, std::string texFilepath) {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture(texFilepath));
	sprites[0].setPosition(spawnPos);

	destination = spawnPos;
	updatePos = spawnPos;
}

Entity::Entity(sf::Vector2f spawnPos, std::vector<std::string> texFilepaths) {
	sprites.resize(texFilepaths.size());
	for (size_t i = 0; i < sprites.size(); i++) {
		sprites[i].setTexture(*assetManager.LoadTexture(texFilepaths[i]));
		sprites[i].setPosition(spawnPos);
	}

	destination = spawnPos;
	updatePos = spawnPos;
}

void Entity::Update(float secondsPerUpdate) {
	if (!sprites.empty() && sprites[0].getPosition() != destination) {
		for (size_t i = 0; i < sprites.size(); i++) {
			sprites[i].setPosition(updatePos);
		}

		updatePos = sprites[0].getPosition();
		velocityPerUpdate = { velocity.x * secondsPerUpdate, velocity.y * secondsPerUpdate };

		// Make sure the entity doesn't overshoot it's destination.
		if (abs(destination.x - sprites[0].getPosition().x) < abs(velocityPerUpdate.x)) {
			updatePos.x = destination.x;
		}
		else {
			updatePos.x += velocityPerUpdate.x;
		}

		// Make sure the entity doesn't overshoot it's destination.
		if (abs(destination.y - sprites[0].getPosition().y) < abs(velocityPerUpdate.y)) {
			updatePos.y = destination.y;
		}
		else {
			updatePos.y += velocityPerUpdate.y;
		}
	}
	else {
		velocity = { 0.f, 0.f };
		velocityPerUpdate = { 0.f, 0.f };
		updatePos = destination;
	}
}

void Entity::Render(sf::RenderTarget& window, float timeRatio) {
	if (!sprites.empty()) {
		if (IsMoving()) {
			sf::Vector2f oldPos = sprites[0].getPosition();
			sf::Vector2f interlopPos = { oldPos.x + velocityPerUpdate.x * timeRatio, oldPos.y + velocityPerUpdate.y * timeRatio };

			// Don't overshoot target destination.
			if (abs(destination.x - oldPos.x) < abs(velocityPerUpdate.x * timeRatio)) {
				interlopPos.x = destination.x;
			}
			if (abs(destination.y - oldPos.y) < abs(velocityPerUpdate.y * timeRatio)) {
				interlopPos.y = destination.y;
			}

			for (size_t i = 0; i < sprites.size(); i++) {
				sprites[i].setPosition(interlopPos);
				if (IsOnScreen(window)) {
					window.draw(sprites[i]);
				}
				sprites[i].setPosition(oldPos);
			}
		}
		else {
			for (size_t i = 0; i < sprites.size(); i++) {
				if (IsOnScreen(window)) {
					window.draw(sprites[i]);
				}
			}
		}
	}
}

void Entity::Move(sf::Vector2f target, float unitsPerSecond) {
	if (!sprites.empty()) {
		if (unitsPerSecond == 0.f) {
			destination = target;
			for (size_t i = 0; i < sprites.size(); i++) {
				sprites[i].setPosition(target);
			}
		}
		else {
			destination = target;
			speed = unitsPerSecond;

			float distance = sqrt(powf(target.x - sprites[0].getPosition().x, 2) + powf(target.y - sprites[0].getPosition().y, 2));
			velocity.x = (unitsPerSecond / distance) * (target.x - sprites[0].getPosition().x);
			velocity.y = (unitsPerSecond / distance) * (target.y - sprites[0].getPosition().y);
		}
	}
}

sf::Vector2f Entity::GetSpritePosition() {
	if (!sprites.empty()) {
		return sprites[0].getPosition();
	}

	return { 0.f, 0.f };
}

bool Entity::IsMoving() {
	if (!sprites.empty()) {
		return (sprites[0].getPosition() != destination);
	}

	return false;
}

bool Entity::IsOnScreen(sf::RenderTarget& window) {
	if (!sprites.empty()) {
		sf::View view = window.getView();
		sf::FloatRect screen(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, view.getSize().x, view.getSize().y);
		sf::FloatRect rect = sprites[0].getGlobalBounds();
		return screen.intersects(rect);
	}
	return false;
}