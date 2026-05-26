#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "TargetComponent.h"
#include "Vector.h"

namespace XYZEngine
{
	/**
	 * @brief AI movement component for enemy entities that chase targets
	 * 
	 * This component implements simple chase behavior:
	 * - Detects target within a specified radius
	 * - Moves towards target when detected
	 * - Stops when target leaves detection radius
	 * 
	 * The component requires:
	 * - TransformComponent (for position and movement)
	 * - TargetComponent (to know what to chase)
	 * 
	 * Usage example:
	 * @code
	 * auto enemyMovement = enemy->AddComponent<EnemyMovementComponent>();
	 * enemyMovement->SetSpeed(150.f);
	 * enemyMovement->SetDetectionRadius(400.f);
	 * enemyMovement->SetTargetComponent(targetComponent);
	 * @endcode
	 */
	class EnemyMovementComponent : public Component
	{
	public:
		EnemyMovementComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		/**
		 * @brief Set movement speed in pixels per second
		 * @param newSpeed Speed value (typically 100-300)
		 */
		void SetSpeed(float newSpeed);
		float GetSpeed() const;

		/**
		 * @brief Set detection radius in pixels
		 * @param radius Distance at which enemy starts chasing
		 */
		void SetDetectionRadius(float radius);
		float GetDetectionRadius() const;

		/**
		 * @brief Link to TargetComponent for tracking
		 * @param targetComp Pointer to TargetComponent
		 */
		void SetTargetComponent(TargetComponent* targetComp);

		/**
		 * @brief Check if enemy is currently chasing target
		 * @return true if target is within detection radius
		 */
		bool IsChasing() const;

	private:
		TransformComponent* transform;      // For position and movement
		TargetComponent* targetComponent;   // For target tracking

		float speed = 200.f;                // Movement speed (pixels/sec)
		float detectionRadius = 300.f;      // Chase activation distance
		bool isChasing = false;             // Current chase state
	};
}
