#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <string>

namespace XYZEngine
{
	/**
	 * @brief Component for tracking and following a target GameObject
	 * 
	 * This component stores a reference to a target GameObject and continuously
	 * updates its position. It can be used by AI systems (like EnemyMovementComponent)
	 * to track player or other entities.
	 * 
	 * Features:
	 * - Direct target assignment via pointer
	 * - Target search by name (useful when target is created after this component)
	 * - Automatic position caching for performance
	 */
	class TargetComponent : public Component
	{
	public:
		TargetComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		/**
		 * @brief Set target directly by GameObject pointer
		 * @param target Pointer to the target GameObject
		 */
		void SetTarget(GameObject* target);

		/**
		 * @brief Set target by name (will search for it in GameWorld)
		 * @param name Name of the target GameObject to find
		 */
		void SetTargetByName(const std::string& name);

		/**
		 * @brief Get the current target GameObject
		 * @return Pointer to target or nullptr if no target
		 */
		GameObject* GetTarget() const;

		/**
		 * @brief Get cached position of the target
		 * @return Target's world position (updated each frame)
		 */
		const Vector2Df& GetTargetPosition() const;

		/**
		 * @brief Check if component has a valid target
		 * @return true if target exists, false otherwise
		 */
		bool HasTarget() const;

	private:
		void TryFindTarget();

		GameObject* target = nullptr;      // Pointer to target GameObject
		std::string targetName;            // Name for deferred target search
		Vector2Df targetPosition = { 0, 0 }; // Cached target position
	};
}
