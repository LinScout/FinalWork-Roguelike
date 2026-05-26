#include "LevelManager.h"
#include "GameLog.h"

namespace XYZRoguelike
{
	void LevelManager::AddLevel(std::shared_ptr<ILevel> level)
	{
		levels.push_back(level);
	}

	void LevelManager::LoadLevel(int levelIndex, std::function<void(ILevel*)> beforeStart)
	{
		if (levelIndex < 0 || levelIndex >= static_cast<int>(levels.size()))
		{
			GAME_LOG_ERROR("Invalid level index: " + std::to_string(levelIndex));
			return;
		}

		if (currentLevelIndex >= 0 && currentLevelIndex < static_cast<int>(levels.size()))
		{
			levels[currentLevelIndex]->Cleanup();
		}

		currentLevelIndex = levelIndex;
		if (beforeStart)
		{
			beforeStart(levels[currentLevelIndex].get());
		}
		levels[currentLevelIndex]->Start();
		GAME_LOG_INFO(std::string("Loaded level: ") + levels[currentLevelIndex]->GetLevelName());
	}

	void LevelManager::LoadNextLevel()
	{
		if (!HasNextLevel())
		{
			GAME_LOG_WARN("No next level available");
			return;
		}
		LoadLevel(currentLevelIndex + 1);
	}

	void LevelManager::LoadPreviousLevel()
	{
		if (currentLevelIndex <= 0)
		{
			GAME_LOG_WARN("No previous level");
			return;
		}
		LoadLevel(currentLevelIndex - 1);
	}

	ILevel* LevelManager::GetCurrentLevel() const
	{
		if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levels.size()))
		{
			return nullptr;
		}
		return levels[currentLevelIndex].get();
	}

	ILevel* LevelManager::GetLevelAt(int levelIndex) const
	{
		if (levelIndex < 0 || levelIndex >= static_cast<int>(levels.size()))
		{
			return nullptr;
		}
		return levels[levelIndex].get();
	}

	int LevelManager::GetCurrentLevelIndex() const
	{
		return currentLevelIndex;
	}

	bool LevelManager::HasNextLevel() const
	{
		return currentLevelIndex >= 0 && currentLevelIndex + 1 < static_cast<int>(levels.size());
	}

	int LevelManager::GetTotalLevels() const
	{
		return static_cast<int>(levels.size());
	}
}
