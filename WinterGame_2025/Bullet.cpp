#include "Bullet.h"
#include "CircleCollider.h"
#include "Dxlib.h"
#include "Game.h"

namespace
{
	constexpr int GRAPH_CUT_W = 16;
	constexpr int GRAPH_CUT_H = 16;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr float COLLIDER_R = 20.0f;

	constexpr float MOVE_SPEED = 20.0f;
}

Bullet::Bullet():
	_isAlive(false),
	_isTurn(false)
{
	_collider = std::make_shared<CircleCollider>(_pos, COLLIDER_R);
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
}

void Bullet::Draw()
{
	DrawRectRotaGraph(_pos.x, _pos.y, GRAPH_CUT_W * 0, GRAPH_CUT_H * 1, GRAPH_CUT_W, GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true,_isTurn);
#ifdef _DEBUG
	DrawCircle(_pos.x, _pos.y, COLLIDER_R, 0xff0000, false);
#endif
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
