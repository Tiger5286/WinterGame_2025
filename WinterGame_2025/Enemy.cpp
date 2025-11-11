#include "Enemy.h"

Enemy::Enemy(int hp, std::shared_ptr<Player> pPlayer) :
	_hp(hp),
	_isAlive(true),
	_isHitChargeShot(false),
	_pPlayer(pPlayer)
{
}

Enemy::Enemy(Vector2 firstPos, int hp, std::shared_ptr<Player> pPlayer) :
	_hp(hp),
	_isAlive(true),
	_isHitChargeShot(false),
	_pPlayer(pPlayer)
{
	_pos = firstPos;
}

Enemy::~Enemy()
{
}
