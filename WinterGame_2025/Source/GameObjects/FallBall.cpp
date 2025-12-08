#include "FallBall.h"
#include "../Utility/CircleCollider.h"
#include "Player.h"

namespace
{
	constexpr int kGraphSize = 16;
	constexpr float kDrawScale = 4.5f;

	constexpr int kAnimNum = 4;
	constexpr int kOneAnimFrame = 6;

	constexpr int kColliderRadius = 30;
}

FallBall::FallBall(std::shared_ptr<Player> pPlayer, int handle):
	_handle(handle),
	_pPlayer(pPlayer)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale);
	_pCollider = std::make_shared<CircleCollider>(_pos, kColliderRadius);
}

FallBall::~FallBall()
{
}

void FallBall::Init()
{
}

void FallBall::Update(Map& map)
{
	Gravity();
	if (!_isBounced)
	{
		if (MapCollision(map).down)
		{
			_isBounced = true;
			_vel.y = -15.0f;
			_pCollider->SetIsEnabled(false);
		}
	}
	else
	{
		_pos += _vel;
	}
	
	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void FallBall::Draw(Vector2 offset)
{
	_nowAnim.Draw(_pos - offset, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}

void FallBall::Shot(Vector2 pos)
{
	_pos = pos;
	_vel = { 0.0f,0.0f };
	_isAlive = true;
	_pCollider->SetIsEnabled(true);
	_isBounced = false;
}
