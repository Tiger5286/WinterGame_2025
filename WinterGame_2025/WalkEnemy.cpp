#include "WalkEnemy.h"
#include "BoxCollider.h"
#include "Dxlib.h"

namespace
{
	constexpr int GRAPH_CUT_W = 32;
	constexpr int GRAPH_CUT_H = 36;
	constexpr float DRAW_SCALE = 4.0f;

	constexpr float COLLIDER_W = 100;
	constexpr float COLLIDER_H = 110;

	constexpr float GROUND_H = 800.0f;
}

WalkEnemy::WalkEnemy()
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

	if (_pos.y > GROUND_H)	// à íuÇ™ínñ ÇÃçÇÇ≥Çè„âÒÇ¡ÇΩÇÁï‚ê≥
	{
		_pos.y = GROUND_H;
		_vel.y = 0.0f;
	}
}

void WalkEnemy::Draw()
{
	DrawRectRotaGraph(_pos.x, _pos.y - GRAPH_CUT_H / 2 * DRAW_SCALE, GRAPH_CUT_W * 0, GRAPH_CUT_H * 0, GRAPH_CUT_W, GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true);
#ifdef _DEBUG
	DrawBox(_pos.x - COLLIDER_W / 2, _pos.y - COLLIDER_H, _pos.x + COLLIDER_W / 2, _pos.y, 0xff0000, false);
#endif
}