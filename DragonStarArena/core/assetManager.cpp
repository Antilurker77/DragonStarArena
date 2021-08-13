// ================================================================
//
// assetManager.cpp
//
// ================================================================

#include "assetManager.hpp"

AssetManager assetManager;

AssetManager::AssetManager() {

}

sf::Image* AssetManager::LoadImage(std::string filepath) {
	sf::Image image;

	// If asset isn't loaded, attempt to load it into the map.
	if (imageMap.find(filepath) == imageMap.end()) {
		if (image.loadFromFile(filepath)) {
			imageMap[filepath] = image;
		}
		else {
			return nullptr;
		}
	}

	return &imageMap.at(filepath);
}

sf::Texture* AssetManager::LoadTexture(std::string filepath) {
	sf::Texture texture;

	// If asset isn't loaded, attempt to load it into the map.
	if (textureMap.find(filepath) == textureMap.end()) {
		if (texture.loadFromFile(filepath)) {
			textureMap[filepath] = texture;
		}
		else {
			return nullptr;
		}
	}

	return &textureMap.at(filepath);
}

sf::Font* AssetManager::LoadFont(std::string filepath) {
	sf::Font font;

	// If asset isn't loaded, attempt to load it into the map.
	if (fontMap.find(filepath) == fontMap.end()) {
		if (font.loadFromFile(filepath)) {
			fontMap[filepath] = font;
		}
		else {
			return nullptr;
		}
	}

	return &fontMap.at(filepath);
}

sf::SoundBuffer* AssetManager::LoadSoundBuffer(std::string filepath) {
	sf::SoundBuffer soundBuffer;

	// If asset isn't loaded, attempt to load it into the map.
	if (soundBufferMap.find(filepath) == soundBufferMap.end()) {
		if (soundBuffer.loadFromFile(filepath)) {
			soundBufferMap[filepath] = soundBuffer;
		}
		else {
			return nullptr;
		}
	}

	return &soundBufferMap.at(filepath);
}