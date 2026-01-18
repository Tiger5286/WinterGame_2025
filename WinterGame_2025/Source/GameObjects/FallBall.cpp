#include "FallBall.h"
#include "../Utility/CircleCollider.h"
#include "Player.h"
#include "Dxlib.h"
#include "../Systems/SoundManager.h"

namespace
{
	// 描画関連
	constexpr int kGraphSize = 16;
	constexpr float kDrawScale = 4.5f;
	// アニメーション関連
	constexpr int kAnimNum = 4;
	constexpr int kOneAnimFrame = 6;
	// 当たり判定
	constexpr int kColliderRadius = 30;
	// このフレーム経過後に跳ねる
	constexpr int kAfterBounceFrame = 30;
	// 跳ねる力
	constexpr float kBouncePower = 15.0f;

	// 落下判定の余白
	constexpr int kFallMargin = 100;
}

FallBall::FallBall(std::shared_ptr<Player> pPlayer, Vector2 mapSize, int handle):
	_handle(handle),
	_pPlayer(pPlayer),
	_kMapSize(mapSize)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale);
	_pCollider = std::make_shared<CircleCollider>(_pos, static_cast<float>(kColliderRadius));
}

FallBall::~FallBall()
{
	// 何もしない
}

void FallBall::Init()
{
	// 何もしない
}

void FallBall::Update(Map& map)
{
	Gravity();
	_frame++;	// 指定したフレーム経過後のみ跳ねるようにする
	if (!_isBounced && _frame > kAfterBounceFrame)
	{
		// 底面がマップに当たったら跳ねる
		HitDirection hitDir = MapCollision(map);
		if (hitDir.down)
		{
			_isBounced = true;
			_vel.y = -kBouncePower;
			_pCollider->SetIsEnabled(false);
			SoundManager::GetInstance().PlaySoundGame("Bounce");
		}
	}
	else
	{
		_pos += _vel;
	}
	
	_pCollider->SetPos(_pos);

	if (_pPlayer == nullptr)
	{
		printfDx("Playerのポインタがnullptrです\n");
	}
	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	// 画面外に出たら消える
	if (_pos.y > _kMapSize.y + kFallMargin)
	{
		_isAlive = false;
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
	_frame = 0;
}
