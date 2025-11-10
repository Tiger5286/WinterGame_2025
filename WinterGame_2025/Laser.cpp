#include "Laser.h"

namespace
{
	constexpr int GRAPH_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr int COLLIDER_WIDTH = 24;

	constexpr int LAUNCHER_ANIM_INDEX = 0;
	constexpr int LASER_ANIM_INDEX = 1;
	constexpr int IMPACT_ANIM_INDEX = 2;
	constexpr int ANIM_NUM = 2;
	constexpr int ONE_ANIM_FRAME = 6;
}

Laser::Laser(Vector2 firstPos, int handle, std::shared_ptr<Player> pPlayer):
	GameObject(firstPos),
	_handle(handle),
	_pPlayer(pPlayer)
{
}

Laser::~Laser()
{
}

void Init()
{

}

void Update(Map& map)
{

}

void Draw(Vector2 offset)
{

}