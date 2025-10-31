#include "Enemy.h"

Enemy::Enemy(int hp) :
	_hp(hp),
	_isAlive(true),
	_isHitChargeShot(false)
{
}

Enemy::~Enemy()
{
}