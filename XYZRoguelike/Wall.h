#pragma once

#include <string>
#include <GameWorld.h>
#include <ResourceSystem.h>
#include "GameObject.h"

namespace XYZRoguelike
{
	class Wall
	{
	public:
		Wall(const XYZEngine::Vector2Df position, const std::string& wallMapName, int textureMapIndex);
	private:
		XYZEngine::GameObject* gameObject;
	};
}

