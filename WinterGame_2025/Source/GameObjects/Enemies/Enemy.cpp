#include "Enemy.h"

namespace
{
	constexpr int kDamageFrameMax = 10;
}

Enemy::Enemy(int hp, std::shared_ptr<Player> pPlayer) :
	_hp(hp),
	_damageFrame(0),
	_isHitChargeShot(false),
	_pPlayer(pPlayer)
{
}

Enemy::~Enemy()
{
}

void Enemy::OnDamage(int damage)
{
	_hp -= damage;
	_damageFrame = kDamageFrameMax;
}

bool Enemy::GetIsAlive() const
{
	return _hp > 0;
}