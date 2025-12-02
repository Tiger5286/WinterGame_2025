#include "Enemy.h"
#include "../../Systems/EffectManager.h"
#include "../../Utility/Collider.h"

namespace
{
	constexpr int kDamageFrameMax = 5;
}

Enemy::Enemy(int hp, std::shared_ptr<Player> pPlayer,std::shared_ptr<EffectManager> pEffectManager) :
	_hp(hp),
	_damageFrame(0),
	_isHitChargeShot(false),
	_pPlayer(pPlayer),
	_pEffectManager(pEffectManager)
{
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage(int damage)
{
	_hp -= damage;
	_damageFrame = kDamageFrameMax;
	if (_hp <= 0)
	{
		_pEffectManager->Create(_pCollider->GetPos(), EffectType::Explosion);
	}
}

bool Enemy::GetIsAlive() const
{
	return _hp > 0;
}

void Enemy::BaseUpdate()
{
	if (_damageFrame > 0)
	{
		_damageFrame--;
	}
}