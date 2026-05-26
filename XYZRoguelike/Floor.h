#pragma once

#include <string>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>
#include <ResourceSystem.h>
#include "GameObject.h"

namespace XYZRoguelike
{
	class Floor
	{
	public:
		Floor(const XYZEngine::Vector2Df& position, const std::string& floorMapName, int textureMapIndex);
	private:
		XYZEngine::GameObject* gameObject;
	};
}

