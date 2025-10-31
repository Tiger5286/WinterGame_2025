#include "Bullet.h"
#include "CircleCollider.h"
#include "Dxlib.h"
#include "Game.h"

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
}

Bullet::Bullet():
	_shotH(-1),
	_chargeShotH(-1),
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

void Bullet::Update()
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

	if (_pos.x < 0 || _pos.x > GlobalConstants::SCREEN_WIDTH)
	{
		_isAlive = false;
	}
	_collider->SetPos(_pos);
}

void Bullet::Draw()
{
	if (_isAlive)
	{
		if (_type == BulletType::NormalShot)
		{
			DrawRectRotaGraph(_pos.x, _pos.y, NORMAL_GRAPH_CUT_W * 0, NORMAL_GRAPH_CUT_H * 1, NORMAL_GRAPH_CUT_W, NORMAL_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _shotH, true, _isTurn);
		}
		else if (_type == BulletType::ChargeShot)
		{
			DrawRectRotaGraph(_pos.x, _pos.y, CHARGE_GRAPH_CUT_W * 0, CHARGE_GRAPH_CUT_H * 1, CHARGE_GRAPH_CUT_W, CHARGE_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _chargeShotH, true, _isTurn);
		}
		
	}
#ifdef _DEBUG
	_collider->Draw();
#endif
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

void Bullet::SetHandle(int shotH,int chargeShotH)
{
	_shotH = shotH;
	_chargeShotH = chargeShotH;
}

void Bullet::SetType(BulletType type)
{
	_type = type;
}

void Bullet::SetAlive(bool isAlive)
{
	_isAlive = isAlive;
}

bool Bullet::GetAlive()
{
	return _isAlive;
}

void Bullet::SetPos(Vector2 pos)
{
	_pos = pos;
}

void Bullet::SetIsTurn(bool isTurn)
{
	_isTurn = isTurn;
}
