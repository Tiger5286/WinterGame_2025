#include "HammerBoss.h"
#include "../../Game.h"
#include "../../Utility/BoxCollider.h"
#include "../../Systems/Camera.h"
#include "../../Systems/EffectManager.h"
#include "../Player.h"

namespace
{
	// 描画関連
	constexpr int kGraphSizeW = 80;
	constexpr int kGraphSizeH = 48;
	const Vector2 kGraphSize = { 80.0f,48.0f };
	constexpr float kDrawScale = 6.0f;

	// アニメーション関連
	constexpr int kIdleAnimIndex = 0;
	constexpr int kIdleAnimNum = 4;
	constexpr int kAttackAnimIndex = 1;
	constexpr int kAttackAnimNum = 21;
	constexpr int kAttackImpactFrameNum = 11;	// ハンマーを振り下ろし、地面にぶつけるアニメーションのフレーム番号
	constexpr int kOneAnimFrame = 8;

	// 当たり判定
	const Vector2 kColliderSize = { 250,180 };
	const Vector2 kWaveAttackColliderSize = { 100,120 };

	// 行動関係
	// idle
	constexpr int kIdleWaitFrame = 60;
	// waveAttack
	constexpr int kImpactOffsetX = 150;
	constexpr int kWaveMoveScale = 4;
	constexpr int kWaveAttackColliderOffsetX = 70;
	constexpr int kWaveEffectIntervalFrame = 20;
	constexpr int kWaveAttackFrame = 400;

	constexpr int kCameraShakeFrame = 15;
	constexpr int kCameraShakePower = 10;

	constexpr int kHp = 100;
	constexpr int kScore = 10000;
}

HammerBoss::HammerBoss(Vector2 firstPos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, std::shared_ptr<Camera> pCamera, SceneManager& sceneManager, int handle) :
	Enemy(kHp, kScore, pPlayer, pEffectManager, sceneManager),
	_handle(handle),
	_pCamera(pCamera)
{
	// 初期位置設定
	_pos = ChipPosToGamePos(firstPos);
	_pos.y += GlobalConstants::kDrawChipSizeHalf;	// チップ半分下にずらす

	// 当たり判定設定
	_pCollider = std::make_shared<BoxCollider>(_pos, kColliderSize);
	_pCollider->SetPosToBox(_pos);

	// 攻撃用当たり判定設定(最初は無効化)
	_pAttackCollider = std::make_shared<BoxCollider>(Vector2(), kWaveAttackColliderSize);
	_pAttackCollider->SetIsEnabled(false);

	// アニメーション設定
	_idleAnim.Init(_handle, kIdleAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_attackAnim.Init(_handle, kAttackAnimIndex, kGraphSize, kAttackAnimNum, kOneAnimFrame, kDrawScale);
	_nowAnim = _idleAnim;
}

HammerBoss::~HammerBoss()
{
}

void HammerBoss::Init()
{
}

void HammerBoss::Update(Map& map)
{
	if (_state == HammerBossState::Idle)
	{
		_frame++;
		if (_frame > kIdleWaitFrame)
		{
			_frame = 0;
			_state = HammerBossState::FallBallAttack;
			_nowAnim = _attackAnim;
		}
	}
	if (_state == HammerBossState::WaveAttack)
	{
		_frame++;
		constexpr int kImpactFrame = kAttackImpactFrameNum * kOneAnimFrame;
		constexpr int kAnimEndFrame = kAttackAnimNum * kOneAnimFrame;
		if (_frame == kImpactFrame)
		{
			_pCamera->Shake(kCameraShakeFrame, kCameraShakePower);
			Vector2 impactPos = { _pos.x - kImpactOffsetX,_pos.y};	// ハンマーの衝撃位置補正
			_pEffectManager->Create(impactPos, EffectType::ExplosionFloor);
			_pAttackCollider->SetPosToBox(impactPos);
			_pAttackCollider->SetIsEnabled(true);
		}
		if (_frame > kImpactFrame)
		{
			Vector2 wavePos = { _pos.x - (_frame - kImpactFrame) * kWaveMoveScale - kImpactOffsetX, _pos.y };
			_pAttackCollider->SetPosToBox({ wavePos.x + kWaveAttackColliderOffsetX,wavePos.y });
			if (_frame % kWaveEffectIntervalFrame == 0)
			{
				_pEffectManager->Create(wavePos, EffectType::ExplosionUpward);
			}
		}
		if (_frame == kAnimEndFrame)
		{
			_nowAnim = _idleAnim;
		}
		if (_frame > kWaveAttackFrame)
		{
			_frame = 0;
			_state = HammerBossState::Idle;
			_pAttackCollider->SetIsEnabled(false);
		}
	}

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}
	if (_pAttackCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void HammerBoss::Draw(Vector2 offset)
{
	Vector2 drawPos(_pos.x - offset.x, _pos.y - offset.y - kGraphSize.y / 2 * kDrawScale);
	_nowAnim.Draw(drawPos, true);
#ifdef _DEBUG
	_pCollider->Draw(offset);
	_pAttackCollider->Draw(offset);
#endif
}