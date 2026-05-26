#include "pch.h"
#include "EnemyMovementComponent.h"
#include "Logger.h"
#include <cmath>

XYZEngine::EnemyMovementComponent::EnemyMovementComponent(GameObject* gameObject)
	: Component(gameObject)
{
	transform = gameObject->GetComponent<TransformComponent>();
}

void XYZEngine::EnemyMovementComponent::Update(float deltaTime)
{
	// Safety checks
	if (targetComponent == nullptr || transform == nullptr)
	{
		return;
	}

	if (!targetComponent->HasTarget())
	{
		isChasing = false;
		return;
	}

	// Calculate distance to target
	Vector2Df enemyPos = transform->GetWorldPosition();
	Vector2Df targetPos = targetComponent->GetTargetPosition();
	Vector2Df direction = targetPos - enemyPos;
	float distance = direction.GetLength();

	// Check if target is within detection radius
	if (distance <= detectionRadius && distance > 0.001f)
	{
		// Log state change
		if (!isChasing)
		{
			LOG_INFO("Enemy detected player and started chasing");
		}
		isChasing = true;

		// Move towards target
		// Normalize direction vector and multiply by speed
		Vector2Df normalizedDir = { direction.x / distance, direction.y / distance };
		Vector2Df movement = normalizedDir * (speed * deltaTime);
		transform->MoveBy(movement);
	}
	else
	{
		// Target out of range - stop chasing
		if (isChasing)
		{
			LOG_INFO("Enemy lost player and stopped");
		}
		isChasing = false;
	}
}

void XYZEngine::EnemyMovementComponent::Render()
{
	// No rendering needed for this component
}

void XYZEngine::EnemyMovementComponent::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

float XYZEngine::EnemyMovementComponent::GetSpeed() const
{
	return speed;
}

void XYZEngine::EnemyMovementComponent::SetDetectionRadius(float radius)
{
	detectionRadius = radius;
}

float XYZEngine::EnemyMovementComponent::GetDetectionRadius() const
{
	return detectionRadius;
}

void XYZEngine::EnemyMovementComponent::SetTargetComponent(TargetComponent* targetComp)
{
	targetComponent = targetComp;
}

bool XYZEngine::EnemyMovementComponent::IsChasing() const
{
	return isChasing;
}
