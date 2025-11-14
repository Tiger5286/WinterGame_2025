#include "Bullet.h"
#include "CircleCollider.h"
#include "Dxlib.h"
#include "Game.h"
#include "Enemy.h"
#include "Map.h"

namespace
{
	constexpr int NORMAL_GRAPH_CUT_W = 16;
	constexpr int NORMAL_GRAPH_CUT_H = 16;
	constexpr int CHARGE_GRAPH_CUT_W = 32;
	constexpr int CHARGE_GRAPH_CUT_H = 32;
	constexpr float DRAW_SCALE = 3.0f;
	constexpr float CHARGE_SHOT_DRAW_SCALE = 3.0f;

	constexpr float NORMAL_COLLIDER_R = 15.0f;
	constexpr float CHARGE_COLLIDER_R = 25.0f;

	constexpr float MOVE_SPEED = 20.0f;

	constexpr int NORMAL_SHOT_DAMAGE = 1;
	constexpr int CHARGE_SHOT_DAMAGE = 5;
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
	_shotImpactAnim.Init(shotH, 2, Vector2(16, 16), 4, 6, 1.0f,false);
	_chargeShotAnim.Init(chargeShotH, 1, Vector2(32, 32), 4, 6, 3.0f);
	_chargeShotImpactAnim.Init(chargeShotH, 2, Vector2(32, 32), 4, 6, 3.0f,false);

	_collider = std::make_shared<CircleCollider>(_pos, NORMAL_COLLIDER_R);
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

void Bullet::Update(Map& map,Vector2 cameraPos)
{
	if (_isTurn)
	{
		_vel = { -MOVE_SPEED,0.0f };
	}
	else
	{
		_vel = { MOVE_SPEED,0.0f };
	}

	if (!_isImpact)
	{
		_pos += _vel;
	}

	// 画面外に出たら消す
	if (_pos.x < cameraPos.x - GlobalConstants::SCREEN_WIDTH / 2 ||
		_pos.x > cameraPos.x + GlobalConstants::SCREEN_WIDTH / 2)
	{
		_isAlive = false;
	}
	_collider->SetPosToBox(_pos);

	Vector2 hitChipPos;	// 未使用
	if (map.IsCollision(_collider,hitChipPos))
	{	// マップに当たったら
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

	// 弾と敵の当たり判定
	for (auto& enemy : _pEnemys)
	{
		if (_collider->CheckCollision(enemy->GetCollider()))	// 敵と当たっているかチェック
		{
			if (_type == BulletType::NormalShot)
			{	// 通常弾ならダメージ与えて弾消える
				enemy->SetHp(enemy->GetHp() - NORMAL_SHOT_DAMAGE);
				_isImpact = true;
			}
			else if (_type == BulletType::ChargeShot)
			{	// チャージ弾なら最初に当たったときだけダメージ与える
				bool isPrevHit = enemy->GetIsHitChargeShot();	// 前のフレームで敵に当たったか
				enemy->SetIsHitChargeShot(true);	// 敵が持っている弾に当たったフラグをつける
				if (enemy->GetIsHitChargeShot() && !isPrevHit)	// 今のフレームで当たっている、かつ前のフレームで当たっていない
				{	// ダメージを与える
					enemy->SetHp(enemy->GetHp() - CHARGE_SHOT_DAMAGE);
				}
			}
		}
		else if (_type == BulletType::ChargeShot)
		{	// チャージショットが当たっていない時に当たったフラグを消す
			enemy->SetIsHitChargeShot(false);
		}
	}

	_nowAnim.Update();
}

void Bullet::Draw(Vector2 offset)
{

	_nowAnim.Draw(_pos - offset, _isTurn);


#ifdef _DEBUG
	_collider->Draw(offset);
#endif
}

void Bullet::Shot(BulletType type, Vector2 shotPos, bool isTurn)
{
	_isImpact = false;
	_isAlive = true;
	_type = type;
	_pos = shotPos;
	_isTurn = isTurn;
	if (_type == BulletType::NormalShot)
	{
		_collider->SetRadius(NORMAL_COLLIDER_R);
		_nowAnim = _shotAnim;
	}
	else if (_type == BulletType::ChargeShot)
	{
		_collider->SetRadius(CHARGE_COLLIDER_R);
		_nowAnim = _chargeShotAnim;
	}
}

void Bullet::SetContext(std::vector<std::shared_ptr<Enemy>> pEnemys)
{
	_pEnemys = pEnemys;
}