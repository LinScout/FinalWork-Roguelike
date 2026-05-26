#include "Wall.h"
#include <SpriteColliderComponent.h>
#include "GameLog.h"

XYZRoguelike::Wall::Wall(const XYZEngine::Vector2Df position, const std::string& wallMapName, int textureMapIndex)
{
	gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Wall");
	auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
	transform->SetWorldPosition(position);

	auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
	renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared(wallMapName, textureMapIndex));
	renderer->SetPixelSize(128, 128);

	auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
	rigidbody->SetKinematic(true);

	gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();
}
