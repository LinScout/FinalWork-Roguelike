#include "Game.h"
#include <assert.h>
#include <algorithm>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"

namespace XYZRoguelike
{

	// Конструктор игры - инициализация таблицы рекордов и главного меню
	Game::Game()
	{
		// Генерация фейковой таблицы рекордов для демонстрации
		recordsTable =
		{
			{"John", SETTINGS.MAX_APPLES / 2},
			{"Jane", SETTINGS.MAX_APPLES / 3 },
			{"Alice", SETTINGS.MAX_APPLES / 4 },
			{"Bob", SETTINGS.MAX_APPLES / 5 },
			{"Clementine", SETTINGS.MAX_APPLES / 5 },
		};

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		SwitchStateTo(GameStateType::MainMenu);  // Запуск с главного меню
	}

	// Деструктор - очистка всех состояний игры
	Game::~Game()
	{
		Shutdown();
	}

	// Обработка событий окна (закрытие, ввод и т.д.)
	void Game::HandleWindowEvents(sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Закрытие окна при нажатии кнопки закрытия
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// Передача события текущему состоянию игры
			if (stateStack.size() > 0)
			{
				stateStack.back().HandleWindowEvent(event);
			}
		}
	}

	// Обновление игры - управление состояниями и их обновление
	bool Game::Update(float timeDelta)
	{
		// Обработка смены состояний
		if (stateChangeType == GameStateChangeType::Switch)
		{
			// Полная смена - удаляем все состояния
			while (stateStack.size() > 0)
			{
				stateStack.pop_back();
			}
		}
		else if (stateChangeType == GameStateChangeType::Pop)
		{
			// Удаляем только текущее состояние (возврат к предыдущему)
			if (stateStack.size() > 0)
			{
				stateStack.pop_back();
			}
		}

		// Инициализация нового состояния если требуется
		if (pendingGameStateType != GameStateType::None)
		{
			stateStack.push_back(GameState(pendingGameStateType, pendingGameStateIsExclusivelyVisible));
		}

		// Сброс флагов смены состояния
		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;

		// Обновление текущего состояния
		if (stateStack.size() > 0)
		{
			stateStack.back().Update(timeDelta);
			return true;  // Игра продолжается
		}

		return false;  // Нет состояний - выход из игры
	}

	// Отрисовка всех видимых состояний игры
	void Game::Draw(sf::RenderWindow& window)
	{
		if (stateStack.size() > 0)
		{
			// Собираем все видимые состояния (до первого эксклюзивного)
			std::vector<GameState*> visibleGameStates;
			for (auto it = stateStack.rbegin(); it != stateStack.rend(); ++it)
			{
				visibleGameStates.push_back(&(*it));
				if (it->IsExclusivelyVisible())
				{
					break;  // Эксклюзивное состояние скрывает все под собой
				}
			}

			// Отрисовываем от нижнего к верхнему
			for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
			{
				(*it)->Draw(window);
			}
		}
	}

	void Game::Shutdown()
	{
		// Shutdown all game states
		while (stateStack.size() > 0)
		{
			stateStack.pop_back();
		}

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
	}

	void Game::PushState(GameStateType stateType, bool isExclusivelyVisible)
	{
		pendingGameStateType = stateType;
		pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		stateChangeType = GameStateChangeType::Push;
	}

	void Game::PopState()
	{
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Pop;
	}

	void Game::ExitGame()
	{
		SwitchStateTo(GameStateType::MainMenu);
	}

	void Game::SwitchStateTo(GameStateType newState)
	{
		pendingGameStateType = newState;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Switch;
	}

	void Game::ShowRecords()
	{
		PushState(GameStateType::Records, true);
	}

	bool Game::IsEnableOptions(GameOptions option) const
	{
		const bool isEnable = ((std::uint8_t)options & (std::uint8_t)option) != (std::uint8_t)GameOptions::Empty;
		return isEnable;
	}

	void Game::SetOption(GameOptions option, bool value)
	{
		if (value) {
			options = (GameOptions)((std::uint8_t)options | (std::uint8_t)option);
		}
		else {
			options = (GameOptions)((std::uint8_t)options & ~(std::uint8_t)option);
		}
	}

	int Game::GetRecordByPlayerId(const std::string& playerId) const
	{
		auto it = recordsTable.find(playerId);
		return it == recordsTable.end() ? 0 : it->second;
	}

	void Game::QuitGame() {
		SwitchStateTo(GameStateType::None);
	}

	void Game::UpdateRecord(const std::string& playerId, int score)
	{
		recordsTable[playerId] = std::max(recordsTable[playerId], score);
	}

	void Game::StartGame()
	{
		SwitchStateTo(GameStateType::Playing);
	}
	
	void Game::PauseGame()
	{
		PushState(GameStateType::ExitDialog, false);
	}
	
	void Game::WinGame()
	{
		PushState(GameStateType::GameWin, false);
	}
	
	void Game::LooseGame()
	{
		PushState(GameStateType::GameOver, false);
	}

	void Game::UpdateGame(float timeDelta, sf::RenderWindow& window)
	{
		HandleWindowEvents(window);
		if (Update(timeDelta))
		{
			// Draw everything here
			// Clear the window first
			window.clear();

			Draw(window);

			// End the current frame, display window contents on screen
			window.display();
		}
		else
		{
			window.close();
		}
	}

	void Game::LoadNextLevel()
	{
		assert(stateStack.back().GetType() == GameStateType::Playing);
		auto playingData = (stateStack.back().GetData<GameStatePlayingData>());
		playingData->LoadNextLevel();
	}
}
