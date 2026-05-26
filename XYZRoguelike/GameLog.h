#pragma once

#include "Logger.h"
#include <memory>

namespace XYZRoguelike
{
	void SetupGameLogger();

#define GAME_LOG_INFO(message) \
	do { \
		auto _logger = LoggerRegistry::getInstance().getLogger("game"); \
		if (_logger) { _logger->info(message); } \
	} while (0)

#define GAME_LOG_WARN(message) \
	do { \
		auto _logger = LoggerRegistry::getInstance().getLogger("game"); \
		if (_logger) { _logger->warn(message); } \
	} while (0)

#define GAME_LOG_ERROR(message) \
	do { \
		auto _logger = LoggerRegistry::getInstance().getLogger("game"); \
		if (_logger) { _logger->error(message); } \
	} while (0)
}
