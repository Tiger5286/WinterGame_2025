#include "FlyEnemy.h"
#include "CircleCollider.h"
#include <cmath>

namespace
{
	// •`‰æŠÖŒW
	constexpr int GRAPH_SIZE_W = 48;
	constexpr int GRAPH_SIZE_H = 48;
	const Vector2 FRAME_SIZE = { GRAPH_SIZE_W, GRAPH_SIZE_H };
	constexpr float DRAW_SCALE = 3.0f;

	// ƒAƒjƒ[ƒVƒ‡ƒ“ŠÖŒW
	constexpr int ANIM_FRAME_NUM = 5;
	constexpr int ONE_ANIM_FRAME = 5;

	// “®‚«ŠÖŒW
	constexpr float WAVE_HEIGHT = 20.0f;
}

FlyEnemy::FlyEnemy(int handle, std::shared_ptr<Player> pPlayer) :
	Enemy(5,pPlayer),
	_handle(handle),
	_angle(0.0f)
{
	_collider = std::make_shared<CircleCollider>(_pos, 35);
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Init()
{
	_nowAnim.Init(_handle, 0, FRAME_SIZE, ANIM_FRAME_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
}

void FlyEnemy::Update()
{
	_nowAnim.Update();


	_pos += _vel;
	_angle += 0.05f;
	const Vector2 adjustPos = { _pos.x, _pos.y + sinf(_angle) * WAVE_HEIGHT };

	_collider->SetPos(adjustPos);
}

void FlyEnemy::Draw()
{
	const Vector2 adjustPos = { _pos.x, _pos.y + sinf(_angle) * WAVE_HEIGHT };
	_nowAnim.Draw(adjustPos, false);
#ifdef _DEBUG
	_collider->Draw();
#endif
}