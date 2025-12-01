#include "DroneEnemy.h"

DroneEnemy::DroneEnemy(Vector2 firstPos, std::shared_ptr<Player> pPlayer, int handle):
	Enemy(2,pPlayer),
	_handle(handle)
{
}

DroneEnemy::~DroneEnemy()
{
}

void DroneEnemy::Init()
{
}

void DroneEnemy::Update(Map& map)
{
}

void DroneEnemy::Draw(Vector2 offset)
{
}