#include "FlyEnemy.h"
#include "CircleCollider.h"
#include <cmath>
#include "Player.h"

namespace
{
	// 描画関係
	constexpr int kGraphSizeW = 48;
	constexpr int kGraphSizeH = 48;
	const Vector2 kFrameSize = { kGraphSizeW, kGraphSizeH };
	constexpr float kDrawScale = 3.0f;

	// アニメーション関係
	constexpr int kAnimFrameNum = 5;
	constexpr int kOneAnimFrame = 5;

	// 動き関係
	constexpr float kWaveHeight = 20.0f;
	constexpr float kAccel = 0.05;
	constexpr float kMaxMoveSpeed = 2.5f;
}

FlyEnemy::FlyEnemy(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, FlyEnemyState state):
	Enemy(5,pPlayer),
	_handle(handle),
	_state(state),
	_angle(0.0f)
{
	_pos = ChipPosToGamePos(chipPos);
	_pCollider = std::make_shared<CircleCollider>(_pos, 35);
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Init()
{
	_nowAnim.Init(_handle, 0, kFrameSize, kAnimFrameNum, kOneAnimFrame, kDrawScale);
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
			_vel.x += kAccel;
		}
		else	// プレイヤーが右にいるとき
		{
			_vel.x -= kAccel;
		}
	}
	// 移動速度の上限を設定
	if (_vel.x > kMaxMoveSpeed) _vel.x = kMaxMoveSpeed;
	if (_vel.x < -kMaxMoveSpeed) _vel.x = -kMaxMoveSpeed;

	_pos += _vel;
	_angle += 0.05f;	// sin用
	const Vector2 adjustPos = { _pos.x, _pos.y + sinf(_angle) * kWaveHeight };	// 上下にsinの動きをつけた位置

	_pCollider->SetPosToBox(adjustPos);

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}
}

void FlyEnemy::Draw(Vector2 offset)
{
	const Vector2 adjustPos = { _pos.x - offset.x, _pos.y - offset.y + sinf(_angle) * kWaveHeight };
	_nowAnim.Draw(adjustPos, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}