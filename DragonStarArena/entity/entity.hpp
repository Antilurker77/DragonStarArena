// ================================================================
//
// entity.hpp
// 
// Base class for any object that needs to move. If an entity is
// using multiple sprites, they will be stacked on top of each
// other.
//
// ================================================================

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../core/assetManager.hpp"

class Entity {
public:
	Entity();
	Entity(sf::Vector2f spawnPos, std::string texFilepath);
	Entity(sf::Vector2f spawnPos, std::vector<std::string> texFilepaths);

	// Updates the entity.
	void Update(float secondsPerUpdate);

	// Draws all the entity's sprites to the screen.
	void Render(sf::RenderTarget& window, float timeRatio);

	// Sets the number of sprites in this entity.
	void SetSpriteCount(size_t count);

	// Sets the texture of the specified sprite.
	void SetTexture(std::string texFilepath, size_t index = 0);

	// Moves the sprite to the specified location. If the velocity is set to 0, the move will be instant.
	void Move(sf::Vector2f target, float unitsPerSecond = 0.f);

	// Randomly alters the position of the sprites.
	void VaryPosition(float amount);

	// Returns the position of the sprite.
	sf::Vector2f GetSpritePosition();

	// Returns true if the entity is currently moving.
	bool IsMoving();

	// Returns true if the entity is on screen.
	bool IsOnScreen(sf::RenderTarget& window);

protected:
	std::vector<sf::Sprite> sprites;
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f velocityPerUpdate = { 0.f, 0.f };
	sf::Vector2f updatePos = { 0.f, 0.f };
	sf::Vector2f destination = { 0.f, 0.f };
	float speed = 0.f;
};