#pragma once

namespace XYZRoguelike
{
	/**
	 * @class ILevel
	 * @brief Интерфейс для всех уровней в игре
	 */
	class ILevel
	{
	public:
		virtual ~ILevel() = default;

		/// Инициализация уровня
		virtual void Start() = 0;

		/// Обновление логики уровня
		virtual void Update(float timeDelta) = 0;

		/// Очистка ресурсов уровня
		virtual void Cleanup() = 0;

		/// Получить название уровня
		virtual const char* GetLevelName() const = 0;

		/// Проверить, завершен ли уровень (победа)
		virtual bool IsLevelComplete() const = 0;

		/// Получить ID следующего уровня
		virtual int GetNextLevelID() const = 0;
	};
}
