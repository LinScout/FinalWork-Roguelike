#include "GameLog.h"

namespace XYZRoguelike
{
	void SetupGameLogger()
	{
		auto gameLogger = std::make_shared<Logger>();
		gameLogger->addSink(std::make_shared<ConsoleSink>());
		gameLogger->addSink(std::make_shared<FileSink>("game_log.txt"));

		LoggerRegistry::getInstance().registerLogger("game", gameLogger);

		GAME_LOG_INFO("Game logger initialized (console and game_log.txt)");
	}
}
