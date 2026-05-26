#include "pch.h"
#include "ColliderComponent.h"

namespace XYZEngine
{
	ColliderComponent::ColliderComponent(GameObject* gameObject) : Component(gameObject) 
	{ 
		
	}

	void ColliderComponent::SetTrigger(bool newIsTrigger)
	{
		isTrigger = newIsTrigger;
	}

	void ColliderComponent::ClearCallbacks()
	{
		onCollisionActions.clear();
		onTriggerEnterActions.clear();
		onTriggerExitActions.clear();
	}

	void ColliderComponent::SubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		onCollisionActions.push_back(onCollisionAction);
	}
	void ColliderComponent::UnsubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		onCollisionActions.erase(std::remove_if
		(
			onCollisionActions.begin(),
			onCollisionActions.end(),
			[&onCollisionAction](const std::function<void(Collision)>& action)
			{
				return action.target<void(Collision)>() == onCollisionAction.target<void(Collision)>();
			}
		), onCollisionActions.end());
	}

	void ColliderComponent::SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		onTriggerEnterActions.push_back(onTriggerEnterAction);
	}
	void ColliderComponent::UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		onTriggerEnterActions.erase(std::remove_if
		(
			onTriggerEnterActions.begin(),
			onTriggerEnterActions.end(),
			[&onTriggerEnterAction](const std::function<void(Trigger)>& action)
			{
				return action.target<void(Trigger)>() == onTriggerEnterAction.target<void(Trigger)>();
			}
		), onTriggerEnterActions.end());
	}

	void ColliderComponent::SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		onTriggerExitActions.push_back(onTriggerExitAction);
	}
	void ColliderComponent::UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		onTriggerExitActions.erase(std::remove_if
		(
			onTriggerExitActions.begin(),
			onTriggerExitActions.end(),
			[&onTriggerExitAction](const std::function<void(Trigger)>& action)
			{
				return action.target<void(Trigger)>() == onTriggerExitAction.target<void(Trigger)>();
			}
		), onTriggerExitActions.end());
	}

	void ColliderComponent::OnCollision(Collision collision)
	{
		// И для обычных коллизий
		auto actionsCopy = onCollisionActions;

		for (int i = 0; i < actionsCopy.size(); i++)
		{
			if (actionsCopy[i])
			{
				actionsCopy[i](collision);
			}
		}
	}
	void ColliderComponent::OnTriggerEnter(Trigger trigger)
	{
		// 1. Делаем локальную КОПИЮ вектора. 
		// Теперь, если оригинальный вектор очистят (ClearCallbacks), 
		// наш цикл продолжит работать с безопасной копией.
		auto actionsCopy = onTriggerEnterActions;

		for (int i = 0; i < actionsCopy.size(); i++)
		{
			if (actionsCopy[i])
			{
				actionsCopy[i](trigger);
			}
		}
	}

	void ColliderComponent::OnTriggerExit(Trigger trigger)
	{
		// То же самое для выхода из триггера
		auto actionsCopy = onTriggerExitActions;

		for (int i = 0; i < actionsCopy.size(); i++)
		{
			if (actionsCopy[i])
			{
				actionsCopy[i](trigger);
			}
		}
	}
}