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
	constexpr float DRAW_SCALE = 2.0f;

	constexpr float NORMAL_COLLIDER_R = 15.0f;
	constexpr float CHARGE_COLLIDER_R = 25.0f;

	constexpr float MOVE_SPEED = 20.0f;

	constexpr int NORMAL_SHOT_DAMAGE = 1;
	constexpr int CHARGE_SHOT_DAMAGE = 5;
}

Bullet::Bullet(Vector2 firstPos,int shotH, int chargeShotH):
	GameObject(firstPos),
	_shotH(shotH),
	_chargeShotH(chargeShotH),
	_type(BulletType::NormalShot),
	_isAlive(false),
	_isTurn(false)
{
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

	_pos += _vel;

	// 画面外に出たら消す
	if (_pos.x < cameraPos.x - GlobalConstants::SCREEN_WIDTH / 2 ||
		_pos.x > cameraPos.x + GlobalConstants::SCREEN_WIDTH / 2)
	{
		_isAlive = false;
	}
	_collider->SetPos(_pos);

	Vector2 hitChipPos;	// 未使用
	if (map.IsCollision(_collider,hitChipPos))
	{	// マップチップに当たったら消える
		_isAlive = false;
	}

	// 弾と敵の当たり判定
	for (auto& enemy : _pEnemys)
	{
		if (_collider->CheckCollision(enemy->GetCollider()))	// 敵と当たっているかチェック
		{
			if (_type == BulletType::NormalShot)
			{	// 通常弾ならダメージ与えて弾消える
				enemy->SetHp(enemy->GetHp() - NORMAL_SHOT_DAMAGE);
				_isAlive = false;
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
}

void Bullet::Draw(Vector2 offset)
{
	if (_isAlive)
	{
		if (_type == BulletType::NormalShot)
		{
			DrawRectRotaGraph(_pos.x - offset.x,
				_pos.y - offset.y,
				NORMAL_GRAPH_CUT_W * 0,
				NORMAL_GRAPH_CUT_H * 1,
				NORMAL_GRAPH_CUT_W,
				NORMAL_GRAPH_CUT_H,
				DRAW_SCALE, 0.0f, _shotH, true, _isTurn);
		}
		else if (_type == BulletType::ChargeShot)
		{
			DrawRectRotaGraph(_pos.x - offset.x,
				_pos.y - offset.y,
				CHARGE_GRAPH_CUT_W * 0,
				CHARGE_GRAPH_CUT_H * 1,
				CHARGE_GRAPH_CUT_W, CHARGE_GRAPH_CUT_H,
				DRAW_SCALE, 0.0f, _chargeShotH, true, _isTurn);
		}
#ifdef _DEBUG
		_collider->Draw(offset);
#endif
	}
}

void Bullet::Shot(BulletType type, Vector2 shotPos, bool isTurn)
{
	_isAlive = true;
	_type = type;
	_pos = shotPos;
	_isTurn = isTurn;
	if (_type == BulletType::NormalShot)
	{
		_collider->SetRadius(NORMAL_COLLIDER_R);
	}
	else if (_type == BulletType::ChargeShot)
	{
		_collider->SetRadius(CHARGE_COLLIDER_R);
	}
}

void Bullet::SetContext(std::vector<std::shared_ptr<Enemy>> pEnemys)
{
	_pEnemys = pEnemys;
}