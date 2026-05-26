#pragma once

#include "Component.h"
#include <SFML/Window.hpp> 

namespace XYZEngine
{
	class InputComponent : public Component
	{
	public:
		InputComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const;
		float GetVerticalAxis() const;
		bool IsAttackPressed() const;
		bool IsUsePotionPressed() const;
		bool IsUseKeyPressed() const;
		bool IsEquipWeaponPressed() const;
		bool IsPrintInventoryPressed() const;

	private:
		float horizontalAxis = 0.f;
		float verticalAxis = 0.f;
		bool attackPressed = false;
		bool usePotionPressed = false;
		bool useKeyPressed = false;
		bool equipWeaponPressed = false;
		bool printInventoryPressed = false;
	};
}