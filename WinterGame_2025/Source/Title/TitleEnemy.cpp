#include "TitleEnemy.h"
#include "Dxlib.h"
#include <cassert>
#include "../Utility/BoxCollider.h"
#include "../Systems/EffectManager.h"
#include "../Systems/SoundManager.h"

namespace
{
	const Vector2 kGraphSize = { 32.0f,36.0f };

	constexpr int kIdleAnimIndex = 0;
	constexpr int kFallAnimIndex = 2;

	constexpr int kIdleAnimNum = 5;
	constexpr int kFallAnimNum = 1;
	constexpr int kOneAnimFrame = 6;
	constexpr float kDrawScale = 3.0f;

	// “–‚½‚è”»’è
	constexpr float kColliderW = 80;
	constexpr float kColliderH = 80;
	const Vector2 kColliderSize = { kColliderW,kColliderH };

	constexpr int kPosX = 1620;
	constexpr int kFloorY = 888;
	constexpr float kMaxVelY = 15.0f;

	constexpr int kDamageFrame = 10;
	constexpr int kMaxHp = 3;
}

TitleEnemy::TitleEnemy(EffectManager& effectManager):
	_effectManager(effectManager)
{
	_handle = LoadGraph("data/Enemys/WalkEnemy.png");
	assert(_handle != -1);

	_pos = { kPosX,-kGraphSize.y * kDrawScale / 2 };

	_pCollider = std::make_shared<BoxCollider>(_pos, kColliderSize);

	_idleAnim.Init(_handle, kIdleAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_fallAnim.Init(_handle, kFallAnimIndex, kGraphSize, kFallAnimNum, kOneAnimFrame, kDrawScale);
	_nowAnim = _fallAnim;
}

TitleEnemy::~TitleEnemy()
{
	DeleteGraph(_handle);
}

void TitleEnemy::Update()
{
	_vel.y++;
	if (_vel.y > kMaxVelY) _vel.y = kMaxVelY;

	_pos += _vel;
	if (_pos.y > kFloorY - kGraphSize.y * kDrawScale / 2)
	{
		_vel = {};
		_pos.y = kFloorY - kGraphSize.y * kDrawScale / 2;
		if (_nowAnim != _idleAnim) _nowAnim = _idleAnim;
	}
	_pCollider->SetPos(_pos);
	_nowAnim.Update();
}

void TitleEnemy::Draw()
{
	if (_damageFrame > 0)
	{
		_damageFrame--;
		SetDrawBright(255, 64, 64);
	}
	_nowAnim.Draw(_pos, true);
	SetDrawBright(255, 255, 255);
#ifdef _DEBUG
	_pCollider->Draw(Vector2());
#endif
}

void TitleEnemy::OnHit()
{
	_hp--;
	_damageFrame = kDamageFrame;
	if (_hp <= 0)
	{
		_effectManager.Create(_pos, EffectType::Explosion);
		_pos = { kPosX,-kGraphSize.y * kDrawScale / 2 };
		_vel = {};
		_hp = kMaxHp;
		_nowAnim = _fallAnim;
		_damageFrame = 0;
		SoundManager::GetInstance().PlaySoundGame("EnemyDeath");
	}
	else
	{
		SoundManager::GetInstance().PlaySoundGame("EnemyDamage");
	}
}