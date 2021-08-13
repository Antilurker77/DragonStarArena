// ================================================================
//
// button.cpp
//
// ================================================================

#include "button.hpp"

#include "../core/assetManager.hpp"
#include "../core/settings.hpp"

const float Button::margin = 8.f;

Button::Button() {
	// Background Setup
	background.setFillColor(sf::Color(0, 0, 0, 255));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));

	// Text Setup
	font = assetManager.LoadFont(settings.Font);

	buttonText.setFont(*font);
	buttonText.setCharacterSize(16);
}

bool Button::Update(float secondsPerUpdate, sf::Vector2i mousePos) {
	if (background.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
		background.setFillColor(sf::Color(255, 255, 255, 255));
		buttonText.setFillColor(sf::Color(0, 0, 0, 255));
		return true;
	}
	else {
		background.setFillColor(sf::Color(0, 0, 0, 255));
		buttonText.setFillColor(sf::Color(255, 255, 255, 255));
	}

	return false;
}

void Button::Render(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(buttonText);
}

void Button::SetString(std::string s) {
	buttonText.setString(s);
	calcSize();
}

void Button::SetString(std::string s, unsigned int fontSize) {
	buttonText.setString(s);
	buttonText.setCharacterSize(fontSize);
	calcSize();
}

void Button::SetPosition(sf::Vector2f pos) {
	SetPosition(pos.x, pos.y);
}

void Button::SetPosition(float x, float y) {
	sf::Vector2f size = background.getSize();
	x -= std::roundf(size.x / 2.f);
	y -= std::roundf(size.y / 2.f);
	background.setPosition(x, y);
	buttonText.setPosition(x + margin, y + margin - (buttonText.getCharacterSize() / 4.f));
}

sf::Vector2f Button::GetSize() {
	return background.getSize();
}

void Button::calcSize() {
	auto size = buttonText.getLocalBounds();
	background.setSize(sf::Vector2f(size.width + (margin * 2.f), size.height + (margin * 2.f)));
}