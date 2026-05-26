#include "pch.h"
#include "InputComponent.h"
#include "Logger.h"

namespace XYZEngine
{
	InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) {}

	void InputComponent::Update(float deltaTime)
	{
		verticalAxis = 0.f;
		horizontalAxis = 0.f;
		attackPressed = false;
		usePotionPressed = false;
		useKeyPressed = false;
		equipWeaponPressed = false;
		printInventoryPressed = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			verticalAxis += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			verticalAxis -= 1.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			horizontalAxis += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			horizontalAxis -= 1.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			attackPressed = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			usePotionPressed = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			useKeyPressed = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			equipWeaponPressed = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
		{
			printInventoryPressed = true;
		}
	}

	bool InputComponent::IsAttackPressed() const
	{
		return attackPressed;
	}

	bool InputComponent::IsUsePotionPressed() const
	{
		return usePotionPressed;
	}

	bool InputComponent::IsUseKeyPressed() const
	{
		return useKeyPressed;
	}

	bool InputComponent::IsEquipWeaponPressed() const
	{
		return equipWeaponPressed;
	}

	bool InputComponent::IsPrintInventoryPressed() const
	{
		return printInventoryPressed;
	}

	void InputComponent::Render()
	{

	}

	float InputComponent::GetHorizontalAxis() const
	{
		return horizontalAxis;
	}
	float InputComponent::GetVerticalAxis() const
	{
		return verticalAxis;
	}
}