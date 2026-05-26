#pragma once

#include "GameplayLevel.h"

namespace XYZRoguelike
{
	// Easy: reach exit without boss.
	class ForestLocation : public GameplayLevel
	{
	public:
		ForestLocation();
	protected:
		void ConfigureLevel() override;
	};

	// Medium: boss with Rush ability.
	class CaveLocation : public GameplayLevel
	{
	public:
		CaveLocation();
	protected:
		void ConfigureLevel() override;
	};

	// Hard: boss with PowerStrike + weapon pickup.
	class CastleLocation : public GameplayLevel
	{
	public:
		CastleLocation();
	protected:
		void ConfigureLevel() override;
	};
}
