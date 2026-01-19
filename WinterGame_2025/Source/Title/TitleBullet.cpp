#include "TitleBullet.h"
#include "Dxlib.h"
#include <cassert>
#include "../Utility/CircleCollider.h"
#include "TitleEnemy.h"
#include "../Game.h"

namespace
{
	const Vector2 kGraphSize = { 16,16 };

	constexpr int kShotAnimIndex = 1;
	constexpr int kImpactAnimIndex = 2;

	constexpr int kIdleAnimNum = 4;
	constexpr int kOneAnimFrame = 6;
	constexpr float kDrawScale = 3.0f;

	const Vector2 kShotPos = { 330,840 };
	const Vector2 kShotVel = { 15.0f,0.0f };
}

TitleBullet::TitleBullet(TitleEnemy& enemy):
	_enemy(enemy)
{
	_handle = LoadGraph("data/Player/Shot.png");
	assert(_handle != -1);

	_pos = kShotPos;
	_pCollider = std::make_shared<CircleCollider>(_pos, 15.0f);
	_pCollider->SetIsEnabled(false);

	_shotAnim.Init(_handle, kShotAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_impactAnim.Init(_handle, kImpactAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale, false);
	_nowAnim = _shotAnim;
}

TitleBullet::~TitleBullet()
{
	DeleteGraph(_handle);
}

void TitleBullet::Update()
{
	if (_isAlive)
	{
		_pos += _vel;
		_pCollider->SetPos(_pos);

		if (_pCollider->CheckCollision(_enemy.GetCollider()))
		{
			OnHit();
			_enemy.OnHit();
		}

		if (_isHit && _nowAnim.GetIsEnd())
		{
			_isAlive = false;
		}

		if (_pos.x > GlobalConstants::kScreenWidth)
		{
			_isAlive = false;
		}

		_nowAnim.Update();
	}
}

void TitleBullet::Draw()
{
	if (_isAlive)
	{
		_nowAnim.Draw(_pos, false);
#ifdef _DEBUG
		_pCollider->Draw(Vector2());
#endif
	}
}

void TitleBullet::Shot()
{
	_isHit = false;
	_isAlive = true;
	_pos = kShotPos;
	_vel = kShotVel;
	_nowAnim = _shotAnim;
	_pCollider->SetIsEnabled(true);
}

void TitleBullet::OnHit()
{
	_vel = Vector2(0.0f, 0.0f);
	_nowAnim = _impactAnim;
	_isHit = true;
	_pCollider->SetIsEnabled(false);
}