#include "Bullet.h"
#include "CircleCollider.h"
#include "Dxlib.h"
#include "Game.h"

namespace
{
	constexpr int PLAYER_GRAPH_CUT_W = 16;
	constexpr int PLAYER_GRAPH_CUT_H = 16;
	constexpr float DRAW_SCALE = 2.0f;

	constexpr float COLLIDER_R = 15.0f;

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
	_collider->SetPos(_pos);
}

void Bullet::Draw()
{
	if (_isAlive)
	{
		DrawRectRotaGraph(_pos.x, _pos.y, PLAYER_GRAPH_CUT_W * 0, PLAYER_GRAPH_CUT_H * 1, PLAYER_GRAPH_CUT_W, PLAYER_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true, _isTurn);
	}
#ifdef _DEBUG
	_collider->Draw();
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
