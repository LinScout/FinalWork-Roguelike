#include "pch.h"
#include "TargetComponent.h"
#include "GameWorld.h"
#include "Logger.h"

XYZEngine::TargetComponent::TargetComponent(GameObject* gameObject)
	: Component(gameObject)
{
}

// Try to find target by name if it wasn't found yet
void XYZEngine::TargetComponent::TryFindTarget()
{
	if (!targetName.empty() && target == nullptr)
	{
		target = GameWorld::Instance()->FindGameObjectByName(targetName);
	}
}

void XYZEngine::TargetComponent::Update(float deltaTime)
{
	// Try to find target if searching by name
	TryFindTarget();

	// Update cached target position
	if (target != nullptr)
	{
		auto targetTransform = target->GetComponent<TransformComponent>();
		if (targetTransform != nullptr)
		{
			targetPosition = targetTransform->GetWorldPosition();
		}
	}
}

void XYZEngine::TargetComponent::Render()
{
	// No rendering needed for this component
}

void XYZEngine::TargetComponent::SetTarget(GameObject* newTarget)
{
	target = newTarget;
	targetName.clear(); // Clear name-based search
}

void XYZEngine::TargetComponent::SetTargetByName(const std::string& name)
{
	targetName = name;
	target = nullptr;
	
	// Try to find immediately
	TryFindTarget();
	
	if (target == nullptr)
	{
		LOG_WARN("Target not found yet: " + name);
	}
	else
	{
		LOG_INFO("Target assigned: " + name);
	}
}

XYZEngine::GameObject* XYZEngine::TargetComponent::GetTarget() const
{
	return target;
}

const XYZEngine::Vector2Df& XYZEngine::TargetComponent::GetTargetPosition() const
{
	return targetPosition;
}

bool XYZEngine::TargetComponent::HasTarget() const
{
	return target != nullptr;
}
