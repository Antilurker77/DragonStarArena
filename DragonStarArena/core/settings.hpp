// ================================================================
//
// settings.hpp
// 
// Global struct that stores setting information.
//
// ================================================================

#pragma once

#include <string>
#include <SFML/Window.hpp>

struct Settings {
	uint32_t ScreenWidth = 1280u;
	uint32_t ScreenHeight = 720u;
	float SceenWidthF = static_cast<float>(ScreenWidth);
	float SceenHeightF = static_cast<float>(ScreenHeight);

	bool EnableVSync = true;
	bool EnableFullscreen = false;
	bool DrawFPS = true;

	std::string Font = "gfx/font/B612Mono-Regular.ttf";
};

extern Settings settings;