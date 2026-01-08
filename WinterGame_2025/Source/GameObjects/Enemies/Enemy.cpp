#include "Enemy.h"
#include "../../Systems/EffectManager.h"
#include "../../Utility/Collider.h"
#include "../../Scenes/SceneManager.h"
#include "../Bullet.h"

namespace
{
	constexpr int kDamageFrameMax = 5;
}

Enemy::Enemy(int hp, int score, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, ItemManager& itemManager, SceneManager& sceneManager) :
	_hp(hp),
	_kScore(score),
	_itemManager(itemManager),
	_sceneManager(sceneManager),
	_damageFrame(0),
	_isHitChargeShot(false),
	_pPlayer(pPlayer),
	_pEffectManager(pEffectManager)
{
}

Enemy::~Enemy()
{
}

void Enemy::UpdateAnytime()
{
}

void Enemy::BaseDraw(const Vector2& offset)
{
	_drawOffset = offset;
}

void Enemy::TakeDamage(int damage,Bullet& bullet)
{
	// ダメージを受ける
	_hp -= damage;
	_damageFrame = kDamageFrameMax;
	// 死んだらエフェクトを出す
	if (_hp <= 0)
	{
		_pEffectManager->Create(GetColliderPos(), EffectType::Explosion);
	}
	// 通常弾なら消す
	if (bullet.GetType() == BulletType::NormalShot)
	{
		bullet.Hit();
	}
}

bool Enemy::GetIsAlive() const
{
	return _hp > 0;
}

const Vector2& Enemy::GetDrawPos() const
{
	return GetColliderPos() + _drawOffset;
}

void Enemy::BaseUpdate()
{
	if (_damageFrame > 0)
	{
		_damageFrame--;
	}
}