#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "ILevel.h"

namespace XYZRoguelike
{
	/**
	 * @class LevelManager
	 * @brief Управляет переходами между уровнями и загрузкой/выгрузкой сцен
	 */
	class LevelManager
	{
	public:
		LevelManager() = default;
		~LevelManager() = default;

		/// Добавить уровень в список
		void AddLevel(std::shared_ptr<ILevel> level);

		/// Загрузить уровень по индексу
		void LoadLevel(int levelIndex, std::function<void(ILevel*)> beforeStart = nullptr);

		/// Загрузить следующий уровень
		void LoadNextLevel();

		/// Переход к предыдущему уровню
		void LoadPreviousLevel();

		/// Получить текущий уровень
		ILevel* GetCurrentLevel() const;
		ILevel* GetLevelAt(int levelIndex) const;

		/// Получить индекс текущего уровня
		int GetCurrentLevelIndex() const;

		/// Проверить, есть ли следующий уровень
		bool HasNextLevel() const;

		/// Общее количество уровней
		int GetTotalLevels() const;

	private:
		std::vector<std::shared_ptr<ILevel>> levels;
		int currentLevelIndex = -1;
	};
}
