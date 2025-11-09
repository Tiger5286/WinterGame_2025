#include "Enemy.h"

Enemy::Enemy(Vector2 firstPos,int hp, std::shared_ptr<Player> pPlayer) :
	GameObject(firstPos),
	_hp(hp),
	_isAlive(true),
	_isHitChargeShot(false),
	_pPlayer(pPlayer)
{
}

Enemy::~Enemy()
{
}
