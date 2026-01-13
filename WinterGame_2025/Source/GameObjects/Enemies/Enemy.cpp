#include "Enemy.h"
#include "../../Systems/EffectManager.h"
#include "../../Utility/Collider.h"
#include "../../Scenes/SceneManager.h"
#include "../Bullet.h"
#include "../Player.h"
#include "../../Systems/ItemManager.h"
#include "../../Systems/SoundManager.h"
#include "Dxlib.h"

namespace
{
	constexpr int kDamageFrameMax = 5;

	// プレイヤーのHPがこれ以下なら回復アイテムを出す
	constexpr int kSummonItemPlayerHp = 2;

	// 回復アイテムを落とす確率(これ分の1)
	constexpr int kSummonItemProbability = 10;
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
	// 死んだら
	if (_hp <= 0)
	{
		// エフェクトを出す
		_pEffectManager->Create(GetColliderPos(), EffectType::Explosion);
		// 敵が死んだ音を鳴らす
		SoundManager::GetInstance().PlaySoundGame("EnemyDeath");
		// プレイヤーのHPが2以下の時1/10の確率で回復を落とす
		if (_pPlayer->GetHp() <= kSummonItemPlayerHp && !GetRand(kSummonItemProbability - 1))	// 0から数えるため 1/10 = 0~9
		{
			// 回復アイテムを出す
			_itemManager.SummonHealthItem(_pCollider->GetPos());
		}
	}
	else
	{
		// ダメージを受けたが死んでいない時被弾音を鳴らす
		SoundManager::GetInstance().PlaySoundGame("EnemyDamage");
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

Vector2 Enemy::GetDrawPos() const
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