#include "pch.h"
#include "Component.h"
#include <iostream>

namespace XYZEngine
{
	// Конструктор компонента - сохраняет ссылку на GameObject владельца
	Component::Component(GameObject* gameObject) : gameObject(gameObject) {}
	
	// Деструктор - выводит информацию об удалении компонента
	Component::~Component()
	{
		std::cout << "Deleted component: " << this << std::endl;
	}
	
	// Возвращает GameObject, к которому прикреплен компонент
	GameObject* Component::GetGameObject()
	{
		return gameObject;
	}
}