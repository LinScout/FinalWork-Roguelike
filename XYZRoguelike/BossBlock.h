#pragma once

#include "ThreeHitBlock.h"

namespace XYZRoguelike
{
	class BossBlock : public ThreeHitBlock
	{
	public:
		BossBlock(const sf::Vector2f& position);
	};
}
