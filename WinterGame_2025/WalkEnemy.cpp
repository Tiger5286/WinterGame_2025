#include "WalkEnemy.h"
#include "BoxCollider.h"
#include "Dxlib.h"
#include "Bullet.h"

namespace
{
	constexpr int PLAYER_GRAPH_CUT_W = 32;
	constexpr int PLAYER_GRAPH_CUT_H = 36;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr float COLLIDER_W = 80;
	constexpr float COLLIDER_H = 80;

	constexpr float GROUND_H = 800.0f;

	constexpr int MAX_HP = 5;
}

WalkEnemy::WalkEnemy():
	Enemy(MAX_HP)
{
	_collider = std::make_shared<BoxCollider>(_pos, Vector2{ COLLIDER_W,COLLIDER_H });
}

WalkEnemy::~WalkEnemy()
{
}

void WalkEnemy::Init()
{
	_pos = { 300.0f,100.0f };
}

void WalkEnemy::Update()
{
	Gravity();

	_pos += _vel;

	if (_pos.y > GROUND_H)	// ˆÊ’u‚ª’n–Ê‚Ì‚‚³‚ðã‰ñ‚Á‚½‚ç•â³
	{
		_pos.y = GROUND_H;
		_vel.y = 0.0f;
	}
	_collider->SetPos(_pos);

	// ’e‚Æ‚Ì“–‚½‚è”»’è
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			if (_collider->CheckCollision(bullet->GetCollider()))
			{
				bullet->SetAlive(false);
				_hp--;
			}
		}
	}
}

void WalkEnemy::Draw()
{
	DrawRectRotaGraph(_pos.x, _pos.y - PLAYER_GRAPH_CUT_H / 2 * DRAW_SCALE, PLAYER_GRAPH_CUT_W * 0, PLAYER_GRAPH_CUT_H * 0, PLAYER_GRAPH_CUT_W, PLAYER_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true);
#ifdef _DEBUG
	_collider->Draw();
#endif
}

void WalkEnemy::SetHandle(int handle)
{
	_handle = handle;
}
