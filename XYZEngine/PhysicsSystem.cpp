#include "pch.h"
#include "PhysicsSystem.h"
#include <algorithm> // Для std::find

namespace XYZEngine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		// 1. Проходим по списку и ОЧИЩАЕМ его от мертвых указателей
		colliders.erase(std::remove(colliders.begin(), colliders.end(), nullptr), colliders.end());

		for (int i = 0; i < colliders.size(); i++)
		{
			if (colliders[i] == nullptr) continue;

			auto* gameObject = colliders[i]->GetGameObject();
			if (gameObject == nullptr) continue;

			auto body = gameObject->GetComponent<RigidbodyComponent>();
			if (body != nullptr && body->GetKinematic())
			{
				continue;
			}

			for (int j = 0; j < colliders.size(); j++)
			{
				if (i == j || colliders[j] == nullptr) continue;
				sf::FloatRect intersection;

				// Запоминаем указатели, чтобы безопасно проверять их дальше
				auto colI = colliders[i];
				auto colJ = colliders[j];

				if (colI->bounds.intersects(colJ->bounds, intersection))
				{
					if (colI->isTrigger != colJ->isTrigger)
					{
						if (triggersEnteredPair.find(colI) == triggersEnteredPair.end() && triggersEnteredPair.find(colJ) == triggersEnteredPair.end())
						{
							Trigger trigger(colI, colJ);

							// Вызываем первый колбэк
							colI->OnTriggerEnter(trigger);

							// ПРОВЕРКА ВЫЖИВАЕМОСТИ: Жив ли еще второй объект?
							if (std::find(colliders.begin(), colliders.end(), colJ) != colliders.end())
							{
								colJ->OnTriggerEnter(trigger);
								triggersEnteredPair.emplace(colI, colJ);
							}
						}
					}
					else if (!colI->isTrigger)
					{
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;
						Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };
						Vector2Df aPosition = { colI->bounds.left, colI->bounds.top };
						auto aTransform = colI->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
							aTransform->MoveBy({ 0, (intersectionPosition.y > aPosition.y) ? -intersectionHeight : intersectionHeight });
						else
							aTransform->MoveBy({ (intersectionPosition.x > aPosition.x) ? -intersectionWidth : intersectionWidth, 0.f });

						Collision collision(colI, colJ, intersection);

						// Вызываем первый колбэк
						colI->OnCollision(collision);

						// ПРОВЕРКА ВЫЖИВАЕМОСТИ: Жив ли еще второй объект?
						if (std::find(colliders.begin(), colliders.end(), colJ) != colliders.end())
						{
							colJ->OnCollision(collision);
						}
					}
				}
			}
		}

		// 2. Очистка triggersEnteredPair перед выходом из триггера
		// Удаляем пары, где хотя бы один объект уже был уничтожен (иначе получим краш в OnTriggerExit)
		for (auto it = triggersEnteredPair.begin(); it != triggersEnteredPair.end(); )
		{
			if (std::find(colliders.begin(), colliders.end(), it->first) == colliders.end() ||
				std::find(colliders.begin(), colliders.end(), it->second) == colliders.end())
			{
				it = triggersEnteredPair.erase(it);
			}
			else
			{
				++it;
			}
		}

		// 3. Обработка OnTriggerExit только для гарантированно живых объектов
		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				Trigger trigger(triggeredPair->first, triggeredPair->second);

				triggeredPair->first->OnTriggerExit(trigger);

				if (std::find(colliders.begin(), colliders.end(), triggeredPair->second) != colliders.end())
				{
					triggeredPair->second->OnTriggerExit(trigger);
				}

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		colliders.push_back(collider);
	}

	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		// Удаляем коллайдер из основного списка
		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());

		// КРИТИЧЕСКИ ВАЖНО: Удаляем любые упоминания этого коллайдера из активных триггеров
		for (auto it = triggersEnteredPair.begin(); it != triggersEnteredPair.end(); )
		{
			if (it->first == collider || it->second == collider)
			{
				it = triggersEnteredPair.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}