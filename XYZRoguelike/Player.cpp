#include "Player.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include <SpriteMovementAnimationComponent.h>
#include <CombatStatsDisplayComponent.h>
#include <AttackAnimationComponent.h>
#include "GameLog.h"

namespace XYZRoguelike
{
	// Конструктор игрока - создает GameObject со всеми необходимыми компонентами
	Player::Player(const XYZEngine::Vector2Df& position)
	{
		GAME_LOG_INFO("Creating player entity");

		// Создаем игровой объект с именем "Player"
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Player");
		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		// Визуальное представление (спрайт игрока)
		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(100, 100);

		// Камера следует за игроком
		auto camera = gameObject->AddComponent<XYZEngine::CameraComponent>();
		camera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());
		camera->SetBaseResolution(1280, 720);

		// Обработка ввода (WASD)
		gameObject->AddComponent<XYZEngine::InputComponent>();

		// Движение на основе ввода
		auto movement = gameObject->AddComponent<XYZEngine::MovementComponent>();
		movement->SetSpeed(400.f);  // Скорость игрока

		// Поворот спрайта в направлении движения
		gameObject->AddComponent<XYZEngine::SpriteDirectionComponent>();

		// Физика (кинематический режим)
		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		// Коллайдер для столкновений
		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		// Анимация ходьбы
		auto animator = gameObject->AddComponent<XYZEngine::SpriteMovementAnimationComponent>();
		animator->Initialize("player", 6.f);  // 6 кадров в секунду

		// Боевые характеристики (HP, броня)
		combatStats = gameObject->AddComponent<XYZEngine::CombatStatsComponent>();
		combatStats->SetMaxHealthPoints(100);
		combatStats->SetHealthPoints(100);
		combatStats->SetArmorPoints(10);
		gameObject->AddComponent<XYZEngine::CombatStatsDisplayComponent>();

		// Система атаки (ручная атака по Space)
		attackComponent = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attackComponent->SetDamage(20);
		attackComponent->SetAttackRange(130.f);
		attackComponent->SetCooldown(0.4f);
		attackComponent->SetAutoAttack(false);  // Ручная атака
		attackComponent->SetTargetByName("Enemy");

		// Анимация атаки (наклон спрайта)
		auto attackAnim = gameObject->AddComponent<XYZEngine::AttackAnimationComponent>();
		attackAnim->SetTiltAngle(20.f);
		attackAnim->SetDuration(0.2f);

		GAME_LOG_INFO("Player ready: HP 100/100, armor 10, attack on Space");
	}

	// Возвращает GameObject игрока
	XYZEngine::GameObject* Player::GetGameObject() const
	{
		return gameObject;
	}

	// Возвращает компонент боевых характеристик
	XYZEngine::CombatStatsComponent* Player::GetCombatStats() const
	{
		return combatStats;
	}

	// Возвращает компонент атаки
	XYZEngine::AttackComponent* Player::GetAttackComponent() const
	{
		return attackComponent;
	}
}
