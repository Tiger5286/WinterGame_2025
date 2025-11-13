#include "FlyEnemy.h"
#include "CircleCollider.h"
#include <cmath>
#include "Player.h"

namespace
{
	// 描画関係
	constexpr int GRAPH_SIZE_W = 48;
	constexpr int GRAPH_SIZE_H = 48;
	const Vector2 FRAME_SIZE = { GRAPH_SIZE_W, GRAPH_SIZE_H };
	constexpr float DRAW_SCALE = 3.0f;

	// アニメーション関係
	constexpr int ANIM_FRAME_NUM = 5;
	constexpr int ONE_ANIM_FRAME = 5;

	// 動き関係
	constexpr float WAVE_HEIGHT = 20.0f;
	constexpr float ACCEL = 0.05;
	constexpr float MAX_MOVE_SPEED = 2.5f;
}

//FlyEnemy::FlyEnemy(int handle, FlyEnemyState state,std::shared_ptr<Player> pPlayer) :
//	Enemy(5, pPlayer),
//	_handle(handle),
//	_angle(0.0f),
//	_state(state)
//{
//	_collider = std::make_shared<CircleCollider>(_pos, 35);
//}
//
//FlyEnemy::FlyEnemy(Vector2 firstPos, int handle, FlyEnemyState state,std::shared_ptr<Player> pPlayer) :
//	Enemy(5, pPlayer),
//	_handle(handle),
//	_angle(0.0f),
//	_state(state)
//{
//	_collider = std::make_shared<CircleCollider>(_pos, 35);
//	_pos = MapChipPosToGamePos(firstPos);
//}

FlyEnemy::FlyEnemy(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, FlyEnemyState state):
	Enemy(5,pPlayer),
	_handle(handle),
	_state(state),
	_angle(0.0f)
{
	_pos = ChipPosToGamePos(chipPos);
	_collider = std::make_shared<CircleCollider>(_pos, 35);
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Init()
{
	_nowAnim.Init(_handle, 0, FRAME_SIZE, ANIM_FRAME_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
}

void FlyEnemy::Update(Map& map)
{
	_nowAnim.Update();

	// ステートがMoveの時はプレイヤーを追う
	if (_state == FlyEnemyState::Move)
	{
		// プレイヤーが左にいるとき
		if (_pos.x < _pPlayer->GetPos().x)
		{
			_vel.x += ACCEL;
		}
		else	// プレイヤーが右にいるとき
		{
			_vel.x -= ACCEL;
		}
	}
	// 移動速度の上限を設定
	if (_vel.x > MAX_MOVE_SPEED) _vel.x = MAX_MOVE_SPEED;
	if (_vel.x < -MAX_MOVE_SPEED) _vel.x = -MAX_MOVE_SPEED;

	_pos += _vel;
	_angle += 0.05f;	// sin用
	const Vector2 adjustPos = { _pos.x, _pos.y + sinf(_angle) * WAVE_HEIGHT };	// 上下にsinの動きをつけた位置

	_collider->SetPosToBox(adjustPos);

	// プレイヤーに当たったらダメージを与える
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}
}

void FlyEnemy::Draw(Vector2 offset)
{
	const Vector2 adjustPos = { _pos.x - offset.x, _pos.y - offset.y + sinf(_angle) * WAVE_HEIGHT };
	_nowAnim.Draw(adjustPos, false);
#ifdef _DEBUG
	_collider->Draw(offset);
#endif
}