#include "pch.h"
#include "GameObject.h"

namespace XYZEngine
{
	// Конструктор по умолчанию - создает GameObject с именем "GameObject"
	// Автоматически добавляет TransformComponent (обязательный компонент)
	GameObject::GameObject()
	{
		name = "GameObject";
		AddComponent<TransformComponent>();
	}
	
	// Конструктор с именем - создает GameObject с заданным именем
	// Автоматически добавляет TransformComponent (обязательный компонент)
	GameObject::GameObject(std::string newName)
	{
		name = newName;
		AddComponent<TransformComponent>();
	}

	// Деструктор - удаляет все компоненты и очищает списки
	GameObject::~GameObject()
	{
		// Удаляем все компоненты
		for (auto component : components)
		{
			delete component;
		}
		components.clear();
		children.clear();
	}

	// Возвращает имя GameObject
	std::string GameObject::GetName() const
	{
		return name;
	}

	// Выводит иерархию GameObject и его компонентов в консоль (для отладки)
	void GameObject::Print(int depth) const
	{
		std::cout << std::string(depth * 2, ' ') << GetName() << std::endl;
		for (auto& component : components)
		{
			std::cout << std::string(depth * 2, ' ') << "::" << component << std::endl;
		}

		// Рекурсивно выводим дочерние объекты
		for (GameObject* child : children)
		{
			child->Print(depth + 1);
		}
	}

	// Обновляет все компоненты GameObject каждый кадр
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			component->Update(deltaTime);
		}
	}
	
	// Отрисовывает все компоненты GameObject
	void GameObject::Render()
	{
		for (auto& component : components)
		{
			component->Render();
		}
	}

	// Добавляет дочерний GameObject (для иерархии трансформаций)
	void GameObject::AddChild(GameObject* child)
	{
		children.push_back(child);
	}
	
	// Удаляет дочерний GameObject из списка детей
	void GameObject::RemoveChild(GameObject* child)
	{
		children.erase(std::remove_if(children.begin(), children.end(), [child](GameObject* obj) { return obj == child; }), children.end());
	}
}