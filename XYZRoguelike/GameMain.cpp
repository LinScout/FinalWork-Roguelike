// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "EngineException.h"
#include "GameLog.h"
#include "GameSession.h"
#include <string>

namespace
{
	void TryLoadTextureMap(const std::string& id, const std::string& path, sf::Vector2u tileSize, int count)
	{
		try
		{
			XYZEngine::ResourceSystem::Instance()->LoadTextureMap(id, path, tileSize, count, false);
			GAME_LOG_INFO("Loaded texture map: " + id);
		}
		catch (const XYZEngine::EngineException& e)
		{
			GAME_LOG_WARN(std::string("Texture map fallback for ") + id + ": " + e.what());
		}
	}

	void TryLoadTexture(const std::string& id, const std::string& path)
	{
		try
		{
			XYZEngine::ResourceSystem::Instance()->LoadTexture(id, path, false);
			GAME_LOG_INFO("Loaded texture: " + id);
		}
		catch (const XYZEngine::EngineException& e)
		{
			GAME_LOG_WARN(std::string("Background fallback for ") + id + ": " + e.what());
		}
	}
}

int main()
{
	XYZEngine::Engine::Instance();
	XYZRoguelike::SetupGameLogger();

	GAME_LOG_INFO("XYZRoguelike starting");
	LOG_INFO("Engine and game loggers are active");

	XYZEngine::RenderSystem::Instance()->SetMainWindow(
		new sf::RenderWindow(sf::VideoMode(1280, 720), "XYZRoguelike"));

	GAME_LOG_INFO("Loading game resources");

	TryLoadTextureMap("player", "Resources/TextureMaps/Player.png", { 48, 63 }, 4);

	// Default tiles (used if per-level packs are missing).
	TryLoadTextureMap("level_floors", "Resources/TextureMaps/Floor.png", { 16, 16 }, 49);
	TryLoadTextureMap("level_walls", "Resources/TextureMaps/Wall.png", { 16, 16 }, 48);

	// Forest / field location.
	TryLoadTextureMap("level_forest_floors", "Resources/TextureMaps/Levels/Forest/Floor.png", { 16, 16 }, 49);
	TryLoadTextureMap("level_forest_walls", "Resources/TextureMaps/Levels/Forest/Wall.png", { 16, 16 }, 48);
	TryLoadTexture("background_forest", "Resources/Textures/Backgrounds/ForestField.png");

	// Cave location.
	TryLoadTextureMap("level_cave_floors", "Resources/TextureMaps/Levels/Cave/Floor.png", { 16, 16 }, 49);
	TryLoadTextureMap("level_cave_walls", "Resources/TextureMaps/Levels/Cave/Wall.png", { 16, 16 }, 48);
	TryLoadTexture("background_cave", "Resources/Textures/Backgrounds/Cave.png");

	// Castle / palace location.
	TryLoadTextureMap("level_castle_floors", "Resources/TextureMaps/Levels/Castle/Floor.png", { 16, 16 }, 49);
	TryLoadTextureMap("level_castle_walls", "Resources/TextureMaps/Levels/Castle/Wall.png", { 16, 16 }, 48);
	TryLoadTexture("background_castle", "Resources/Textures/Backgrounds/Palace.png");

	TryLoadTexture("background_default", "Resources/TextureMaps/Floor.png");

	try
	{
		XYZEngine::ResourceSystem::Instance()->LoadSound("music", "Resources/Sounds/OST.wav");
	}
	catch (const XYZEngine::EngineException& e)
	{
		GAME_LOG_ERROR(std::string("Sound load failed: ") + e.what());
	}

	XYZRoguelike::GameSession::Instance().Initialize();

	XYZEngine::Engine::Instance()->SetGameUpdate([](float dt)
	{
		XYZRoguelike::GameSession::Instance().Update(dt);
	});

	XYZEngine::Engine::Instance()->SetOverlayDraw([](sf::RenderWindow& window)
	{
		XYZRoguelike::GameSession::Instance().DrawHud(window);
	});

	XYZEngine::Engine::Instance()->Run();

	GAME_LOG_INFO("XYZRoguelike finished");
	LOG_INFO("Engine shutdown");
	return 0;
}
