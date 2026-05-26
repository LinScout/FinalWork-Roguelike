#include "BossBlock.h"

namespace XYZRoguelike
{
	BossBlock::BossBlock(const sf::Vector2f& position)
		: ThreeHitBlock(position)
	{
		hitCount = 5;
		sprite.setColor(sf::Color::Yellow);
	}
}
