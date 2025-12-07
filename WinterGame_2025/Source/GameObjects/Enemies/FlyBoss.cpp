#include "FlyBoss.h"
#include "Dxlib.h"
#include "../../Utility/CircleCollider.h"
#include "../Player.h"
#include <cmath>

namespace
{
	// 描画関連
	constexpr int kGraphSize = 48;
	constexpr float kDrawScale = 6.0f;

	// アニメーション関連
	constexpr int kAnimNum = 5;
	constexpr int kOneAnimFrame = 6;

	// 当たり判定
	constexpr int kColliderRadius = 70;

	// 動きの制御関連
	constexpr float kSinAngleIncrement = 0.03;
	constexpr int kSinAngleScale = 40;

	constexpr int kHp = 100;
	constexpr int kScore = 10000;
}

FlyBoss::FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager,int handle):
	Enemy(kHp, kScore,pPlayer,pEffectManager,sceneManager),
	_handle(handle)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale);
	_pos = ChipPosToGamePos(pos);
	_pCollider = std::make_shared<CircleCollider>(_pos, kColliderRadius);
}

FlyBoss::~FlyBoss()
{
}

void FlyBoss::Init()
{
}

void FlyBoss::Update(Map& map)
{
	Update();
}

void FlyBoss::UpdateAnytime()
{
	if (_state == FlyBossState::AimTackle)
	{
		//if ()
	}
}

void FlyBoss::Update()
{
	if (_state == FlyBossState::Idle)
	{
		_frame++;
		if (_frame > 60)
		{
			_state = FlyBossState::AimTackle;
			_frame = 0;
		}
	}
	else if (_state == FlyBossState::AimTackle)
	{
		_frame++;
		if (_frame < 120)
		{
			_pos.y = std::lerp(_pos.y, _pPlayer->GetColliderPos().y, 0.05f);
		}
		else if (_frame == 120)
		{
			_isPlayerOnLeft = _pos.x - _pPlayer->GetPos().x > 0;
			_isPlayerOnLeft ? _vel.x = 15.0f : _vel.x = -15.0f;
		}
		else
		{
			if (_isPlayerOnLeft)
			{
				_vel.x -= 1.0f;
			}
			else
			{
				_vel.x += 1.0f;
			}
		}
	}

	_pos += _vel;

	// 縦揺れの位置を計算
	_sinAngle += kSinAngleIncrement;
	Vector2 adjustPos = _pos;
	adjustPos.y = _pos.y + sinf(_sinAngle) * kSinAngleScale;

	_pCollider->SetPos(adjustPos);
	_nowAnim.Update();
}

void FlyBoss::Draw(Vector2 offset)
{
	// コライダーが縦揺れを適用した位置にいるのでそのまま使う
	_nowAnim.Draw(GetColliderPos() - offset, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
	DrawFormatString(_pos.x, _pos.y - 60, 0xffffff, "_frame:%d", _frame);
	//DrawFormatString(_pos.x, _pos.y - 80, 0xffffff, "_vel.x:%.2f", _vel.x);
#endif
}
