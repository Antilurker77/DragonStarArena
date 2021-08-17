// ================================================================
//
// resourceWindow.cpp
//
// ================================================================

#include "resourceWindow.hpp"

#include "../core/assetManager.hpp"
#include "../core/settings.hpp"

ResourceWindow::ResourceWindow() :
	goldIcon(sf::Vector2f(0.f, 0.f), "gfx/icon/item/gold.png"),
	dustIcon(sf::Vector2f(0.f, 0.f), "gfx/icon/item/scroll.png")
{
	goldText.setString("#gold 0");
	goldText.setCharacterSize(16u);
	goldText.setFont(*assetManager.LoadFont(settings.Font));

	dustText.setString("#dust 0");
	dustText.setCharacterSize(16u);
	dustText.setFont(*assetManager.LoadFont(settings.Font));

	background.setFillColor(sf::Color(0, 0, 0, 255));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));

	calcSize();
}

void ResourceWindow::Render(sf::RenderTarget& window) {
	window.draw(background);
	goldIcon.Render(window, 0.f);
	window.draw(goldText);
	dustIcon.Render(window, 0.f);
	window.draw(dustText);
}

void ResourceWindow::SetPosition(float x, float y) {
	x = std::roundf(x);
	y = std::roundf(y);

	SetPosition(sf::Vector2f(x, y));
}

void ResourceWindow::SetPosition(sf::Vector2f pos) {
	background.setPosition(pos);

	pos.x += 2.f;
	pos.y += 2.f;
	goldIcon.Move(pos);

	pos.x += 16.f + 2.f;
	pos.y -= 2.f;
	goldText.setPosition(pos);

	pos.x += goldText.getLocalBounds().width + 8.f;
	pos.y += 2.f;
	dustIcon.Move(pos);

	pos.x += 16.f + 2.f;
	pos.y -= 2.f;
	dustText.setPosition(pos);
}

void ResourceWindow::SetGold(int64_t amount) {
	goldText.setString("#gold " + std::to_string(amount));
	calcSize();
}

void ResourceWindow::SetDust(int64_t amount) {
	dustText.setString("#dust " + std::to_string(amount));
	calcSize();
}

sf::Vector2f ResourceWindow::GetSize() {
	return background.getSize();
}

void ResourceWindow::calcSize() {
	sf::Vector2f size{};
	size.x = 2.f + 16.f + 2.f + goldText.getLocalBounds().width + 8.f + 16.f + 2.f + dustText.getLocalBounds().width + 2.f;
	size.y = 20.f;
	background.setSize(size);
}