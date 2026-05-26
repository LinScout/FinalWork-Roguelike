#include "Floor.h"
#include "GameLog.h"

XYZRoguelike::Floor::Floor(const XYZEngine::Vector2Df& position, const std::string& floorMapName, int textureMapIndex)
{
	gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Floor");
	auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
	transform->SetWorldPosition(position);

	auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
	renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared(floorMapName, textureMapIndex));
	renderer->SetPixelSize(128, 128);
}
