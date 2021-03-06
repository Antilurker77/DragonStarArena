// ================================================================
//
// fpsWindow.hpp
// 
// Window that displays FPS and frame time.
//
// ================================================================

#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class FPSWindow {
public:
	FPSWindow();

	// Updates the window.
	void Update(sf::Time& t);

	// Renders the window.
	void Render(sf::RenderTarget& window);

private:
	sf::RectangleShape background;
	sf::Text fpsText;
	const unsigned int fontSize = 16u;

	sf::Time fpsUpdateTime;
	float numberOfFrames = 0.f;
};
