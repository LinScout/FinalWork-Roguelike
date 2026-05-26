#include "pch.h"
#include "GameWorld.h"
#include "Logger.h"

namespace XYZEngine
{
	// Singleton - возвращает единственный экземпляр GameWorld
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}

	// Обновляет все GameObject в мире каждый кадр
	void GameWorld::Update(float deltaTime)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Update(deltaTime);
		}
	}
	
	// Обновление физики с фиксированным шагом времени
	void GameWorld::FixedUpdate(float deltaTime)
	{
		fixedCounter += deltaTime;
		if (fixedCounter > PhysicsSystem::Instance()->GetFixedDeltaTime())
		{
			fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
			PhysicsSystem::Instance()->Update();
		}
	}
	
	// Отрисовывает все GameObject в мире
	void GameWorld::Render()
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Render();
		}
	}
	
	// Удаляет помеченные на удаление объекты (вызывается в конце кадра)
	void GameWorld::LateUpdate()
	{
		for (int i = markedToDestroyGameObjects.size() - 1; i >= 0; i--)
		{
			DestroyGameObjectImmediate(markedToDestroyGameObjects[i]);
		}
	}

	// Создает новый GameObject с именем по умолчанию
	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* newGameObject = new GameObject();
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}
	
	// Создает новый GameObject с заданным именем
	GameObject* GameWorld::CreateGameObject(std::string name)
	{
		GameObject* newGameObject = new GameObject(name);
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}
	
	// Помечает GameObject на удаление (будет удален в LateUpdate)
	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		markedToDestroyGameObjects.push_back(gameObject);
	}
	
	// Ищет GameObject по имени (возвращает первый найденный или nullptr)
	GameObject* GameWorld::FindGameObjectByName(const std::string& name) const
	{
		for (GameObject* obj : gameObjects)
		{
			if (obj != nullptr && obj->GetName() == name)
			{
				return obj;
			}
		}
		return nullptr;
	}

	// Очищает весь игровой мир - удаляет все GameObject
	void GameWorld::Clear()
	{
		LOG_INFO("Clearing game world");
		for (int i = gameObjects.size() - 1; i >= 0; i--)
		{
			if (gameObjects[i] == nullptr)
			{
				continue;
			}

			// Удаляем только корневые объекты (без родителя)
			// Дочерние объекты удалятся автоматически
			if (gameObjects[i]->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				DestroyGameObjectImmediate(gameObjects[i]);
			}
		}

		fixedCounter = 0.f;
	}

	// Выводит иерархию всех GameObject в консоль (для отладки)
	void GameWorld::Print() const
	{
		for (auto& obj : gameObjects)
		{
			if (obj == nullptr)
			{
				continue;
			}
			// Выводим только корневые объекты (дочерние выведутся рекурсивно)
			if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				obj->Print();
			}
		}
	}

	// Немедленно удаляет GameObject и все его дочерние объекты
	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		if (gameObject == nullptr) return;

		// 1. Получаем все коллайдеры для отписки от физики
		auto colliders = gameObject->GetComponentsInChildren<ColliderComponent>();

		// 2. Отписываем от физики и очищаем коллбэки
		for (auto* c : colliders) {
			PhysicsSystem::Instance()->Unsubscribe(c);
			c->ClearCallbacks();
		}

		// 3. Отсоединяем от родителя если есть
		auto parent = gameObject->GetComponent<TransformComponent>()->GetParent();
		if (parent != nullptr)
		{
			parent->GetGameObject()->RemoveChild(gameObject);
		}

		// 4. Собираем все дочерние объекты для удаления (обход в глубину)
		std::vector<GameObject*> toDelete;
		for (auto transform : gameObject->GetComponentsInChildren<TransformComponent>())
		{
			toDelete.push_back(transform->GetGameObject());
		}

		// 5. Удаляем из векторов и освобождаем память
		for (auto* obj : toDelete)
		{
			gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), obj), gameObjects.end());
			markedToDestroyGameObjects.erase(std::remove(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(), obj), markedToDestroyGameObjects.end());
			delete obj;
		}
	}
}