#include "WalkEnemy.h"
#include "BoxCollider.h"
#include "Dxlib.h"
#include "Bullet.h"

namespace
{
	constexpr int GRAPH_CUT_W = 32;
	constexpr int GRAPH_CUT_H = 36;
	constexpr float DRAW_SCALE = 4.0f;

	constexpr float COLLIDER_W = 100;
	constexpr float COLLIDER_H = 110;

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
	DrawRectRotaGraph(_pos.x, _pos.y - GRAPH_CUT_H / 2 * DRAW_SCALE, GRAPH_CUT_W * 0, GRAPH_CUT_H * 0, GRAPH_CUT_W, GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true);
#ifdef _DEBUG
	_collider->Draw();
#endif
}