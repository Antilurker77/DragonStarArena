// ================================================================
//
// resourceWindow.hpp
// 
// Window that displays the party's gold and dust.
//
// ================================================================

#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "richText.hpp"
#include "../entity/entity.hpp"

class ResourceWindow {
public:
	ResourceWindow();

	// Draws the window to the screen.
	void Render(sf::RenderTarget& window);

	// Sets the position of the resource window.
	void SetPosition(float x, float y);

	// Sets the position of the resource window.
	void SetPosition(sf::Vector2f pos);

	// Sets the gold text.
	void SetGold(int64_t amount);

	// Sets the dust text.
	void SetDust(int64_t amount);

	// Returns the size of the window.
	sf::Vector2f GetSize();

private:
	// Calculates the size of the window.
	void calcSize();

	// ================================
	
	Entity goldIcon;
	Entity dustIcon;
	sfe::RichText goldText;
	sfe::RichText dustText;

	sf::RectangleShape background;
};