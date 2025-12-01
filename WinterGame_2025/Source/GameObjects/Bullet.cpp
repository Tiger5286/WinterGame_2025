#include "Bullet.h"
#include "../Utility/CircleCollider.h"
#include "Dxlib.h"
#include "../Game.h"
#include "Enemies/Enemy.h"
#include "../Systems/Map.h"

namespace
{
	constexpr int kNormalGraphCutW = 16;
	constexpr int kNormalGraphCutH = 16;
	constexpr int kChargeGraphCutW = 32;
	constexpr int kChargeGraphCutH = 32;
	constexpr float kDrawScale = 3.0f;
	constexpr float kChargeShotDrawScale = 3.0f;

	constexpr float kNormalColliderR = 15.0f;
	constexpr float kChargeColliderR = 25.0f;

	constexpr float kMoveSpeed = 20.0f;

	constexpr int kNormalShotDamage = 1;
	constexpr int kChargeShotDamage = 5;
}

Bullet::Bullet(int shotH, int chargeShotH):
	_shotH(shotH),
	_chargeShotH(chargeShotH),
	_type(BulletType::NormalShot),
	_isImpact(false),
	_isAlive(false),
	_isTurn(false)
{
	_shotAnim.Init(shotH, 1, Vector2(16, 16), 4, 6, 3.0f);
	_shotImpactAnim.Init(shotH, 2, Vector2(16, 16), 4, 4, 3.0f,false);
	_chargeShotAnim.Init(chargeShotH, 1, Vector2(32, 32), 4, 6, 3.0f);
	_chargeShotImpactAnim.Init(chargeShotH, 2, Vector2(32, 32), 4, 6, 3.0f,false);

	_pCollider = std::make_shared<CircleCollider>(_pos, kNormalColliderR);
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
}

void Bullet::Update(Map& map)
{
	printfDx("Bullet::Update(Map& map)が呼ばれました。Bullet::Update(Map& map, Vector2 cameraPos)を使用してください。\n");
}

void Bullet::Update(Map& map,Vector2 cameraPos, std::vector<std::shared_ptr<Enemy>> pEnemys)
{
	if (_isTurn)
	{
		_vel = { -kMoveSpeed,0.0f };
	}
	else
	{
		_vel = { kMoveSpeed,0.0f };
	}

	if (!_isImpact)
	{
		_pos += _vel;
	}

	// 画面外に出たら消す
	if (_pos.x < cameraPos.x - GlobalConstants::kScreenWidth / 2 ||
		_pos.x > cameraPos.x + GlobalConstants::kScreenWidth / 2)
	{
		_pCollider->SetIsEnabled(false);
		_isAlive = false;
	}
	_pCollider->SetPosToBox(_pos);

	Vector2 hitChipPos;	// 未使用
	if (map.IsCollision(_pCollider,hitChipPos))
	{	// マップに当たったら
		Hit();
	}

	// 弾と敵の当たり判定
	for (auto& enemy : pEnemys)
	{
		if (_pCollider->CheckCollision(enemy->GetCollider()))	// 敵と当たっているかチェック
		{
			if (_type == BulletType::NormalShot)
			{	// 通常弾ならダメージ与えて弾消える
				enemy->SetHp(enemy->GetHp() - kNormalShotDamage);
				Hit();
			}
			else if (_type == BulletType::ChargeShot)
			{	// チャージ弾なら最初に当たったときだけダメージ与える
				bool isPrevHit = enemy->GetIsHitChargeShot();	// 前のフレームで敵に当たったか
				enemy->SetIsHitChargeShot(true);	// 敵が持っている弾に当たったフラグをつける
				if (enemy->GetIsHitChargeShot() && !isPrevHit)	// 今のフレームで当たっている、かつ前のフレームで当たっていない
				{	// ダメージを与える
					enemy->SetHp(enemy->GetHp() - kChargeShotDamage);
				}
			}
		}
		else if (_type == BulletType::ChargeShot)
		{	// チャージショットが当たっていない時に当たったフラグを消す
			enemy->SetIsHitChargeShot(false);
		}
	}

	if (_isImpact && _nowAnim.GetIsEnd())
	{	// 衝突アニメーションが終わったら消す
		_isAlive = false;
	}

	_nowAnim.Update();
}

void Bullet::Draw(Vector2 offset)
{
	// 弾の描画
	if (_isAlive) _nowAnim.Draw(_pos - offset, _isTurn);

#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}

void Bullet::Shot(BulletType type, Vector2 shotPos, bool isTurn)
{
	_pCollider->SetIsEnabled(true);
	_isImpact = false;
	_isAlive = true;
	_type = type;
	_pos = shotPos;
	_isTurn = isTurn;
	if (_type == BulletType::NormalShot)
	{
		_pCollider->SetRadius(kNormalColliderR);
		_nowAnim = _shotAnim;
	}
	else if (_type == BulletType::ChargeShot)
	{
		_pCollider->SetRadius(kChargeColliderR);
		_nowAnim = _chargeShotAnim;
	}
}

//void Bullet::SetContext(std::vector<std::shared_ptr<Enemy>> pEnemys)
//{
//	_pEnemys = pEnemys;
//}

void Bullet::Hit()
{
	if (!_isImpact)
	{
		if (_type == BulletType::NormalShot)
		{
			ChangeAnim(_shotImpactAnim);
		}
		else if (_type == BulletType::ChargeShot)
		{
			ChangeAnim(_chargeShotImpactAnim);
		}
		_nowAnim.SetFirst();
		_isImpact = true;
	}
	_pCollider->SetIsEnabled(false);	// 当たり判定無効化
}
