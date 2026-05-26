#pragma once
#define NOMINMAX

#include "SFML/Graphics.hpp"
#include <functional>

namespace XYZEngine
{
	class Engine
	{
	public:
		using OverlayDrawCallback = std::function<void(sf::RenderWindow&)>;

		Engine(const Engine& app) = delete;
		Engine& operator= (const Engine&) = delete;

		static Engine* Instance();

		void Run();
		void SetOverlayDraw(OverlayDrawCallback callback);
		void SetGameUpdate(std::function<void(float)> callback);

	private:
		Engine();
		~Engine() = default;

		void setupLogger();

		OverlayDrawCallback overlayDraw;
		std::function<void(float)> gameUpdate;
	};
}