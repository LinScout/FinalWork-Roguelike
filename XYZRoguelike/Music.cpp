#include "Music.h"
#include "EngineException.h"
#include "GameLog.h"

Music::Music(const std::string& soundName)
{
	GAME_LOG_INFO("Starting music: " + soundName);
	try
	{
		auto gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Music: " + soundName);
		auto music = gameObject->AddComponent<XYZEngine::AudioComponent>();
		music->SetAudio(*XYZEngine::ResourceSystem::Instance()->GetSound(soundName));
		music->SetLoop(true);
		music->Play();
		GAME_LOG_INFO("Music playback started");
	}
	catch (const XYZEngine::EngineException& e)
	{
		GAME_LOG_ERROR(std::string("Music failed to start: ") + e.what());
		GAME_LOG_WARN("Game continues without music");
	}
}
