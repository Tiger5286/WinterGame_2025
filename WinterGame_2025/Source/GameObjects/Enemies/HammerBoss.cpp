#include "HammerBoss.h"
#include "Dxlib.h"
#include "../../Game.h"
#include "../../Utility/BoxCollider.h"
#include "../../Systems/Camera.h"
#include "../../Systems/EffectManager.h"
#include "../../Systems/BulletManager.h"
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
	constexpr int kRapidOneAnimFrame = 3; // 速い攻撃アニメーションの1フレームあたりの表示フレーム数

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
	// fallBallAttack
	constexpr int kFBPosRimitLeft = 150;
	constexpr int kFBPosRimitRight = GlobalConstants::kScreenWidth - 600;
	constexpr int kFBAreaNum = 4;
	constexpr int kFBNum = 5;	// 1回の攻撃で落とすボールの数
	constexpr int kFBIntervalFrame = 3;	// ボールを落とす間隔(フレーム数)
	constexpr int kDropNum = 6;	// ボールを落とす回数

	// カメラ揺れ
	constexpr int kCameraShakeFrame = 15;
	constexpr int kCameraShakePowerStrong = 10;
	constexpr int kCameraShakePowerWeak = 5;

	constexpr int kHp = 50;
	constexpr int kScore = 10000;

	constexpr int kDeathFrame = 120;
	constexpr int kDeathFrickerFrame = 10;
	constexpr int kDeathExplosionGapDis = 100;
	constexpr int kAliveTimeBeforeDeath = 210;
}

HammerBoss::HammerBoss(Vector2 firstPos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, BulletManager& bulletManager, std::shared_ptr<Camera> pCamera, SceneManager& sceneManager, int handle) :
	Enemy(kHp, kScore, pPlayer, pEffectManager, sceneManager),
	_handle(handle),
	_pCamera(pCamera),
	_bulletManager(bulletManager)
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
	_rapidAttackAnim.Init(_handle, kAttackAnimIndex, kGraphSize, kAttackAnimNum, kRapidOneAnimFrame, kDrawScale); // 速い攻撃アニメーション
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
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_0))
	{
		_hp = 1;
	}
#endif
	BaseUpdate();
	if (_state == HammerBossState::Idle)
	{
		_frame++;
		if (_frame > kIdleWaitFrame)
		{
			_frame = 0;
			switch (GetRand(1))
			{
			case 0:
				_state = HammerBossState::WaveAttack;
				_nowAnim = _attackAnim;
				break;
			case 1:
				_state = HammerBossState::FallBallAttack;
				_nowAnim = _rapidAttackAnim;
				break;
			}
		}
	}
	if (_state == HammerBossState::WaveAttack)
	{
		_frame++;
		constexpr int kImpactFrame = kAttackImpactFrameNum * kOneAnimFrame;
		constexpr int kAnimEndFrame = kAttackAnimNum * kOneAnimFrame;
		if (_frame == kImpactFrame)
		{
			_pCamera->Shake(kCameraShakeFrame, kCameraShakePowerStrong);
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
	else if (_state == HammerBossState::FallBallAttack)
	{
		_frame++;
		constexpr int kImpactFrame = kAttackImpactFrameNum * kRapidOneAnimFrame;
		constexpr int kAnimEndFrame = kAttackAnimNum * kRapidOneAnimFrame;
		// ハンマーを地面にぶつけたときにカメラを揺らす
		if (_frame % kAnimEndFrame == kImpactFrame)
		{
			_pCamera->Shake(kCameraShakeFrame, kCameraShakePowerWeak);
			_fBArea = GetRand(kFBAreaNum - 1);
		}
		// 一定間隔で落下ボールを発射
		for (int i = 0; i < kFBNum; i++)
		{
			// ボールの落ちてくる位置で最も左側のX座標
			float fallPosXLeft = kFBPosRimitRight / kFBAreaNum * _fBArea + kFBPosRimitLeft;
			// ボールの落ちてくる位置で最も右側のX座標
			float fallPosXRight = kFBPosRimitRight / kFBAreaNum * _fBArea + kFBPosRimitLeft + kFBPosRimitRight / kFBAreaNum;

			// left ~ right の範囲でランダムに落下位置を決定
			auto fallPosX = fallPosXLeft + GetRand(fallPosXRight - fallPosXLeft);
			if (_frame % kAnimEndFrame == kImpactFrame + i * kFBIntervalFrame) _bulletManager.ShotFallBall({ static_cast<float>(fallPosX), 0.0f });
		}

		// kDropNum回ボールを落としたら状態遷移
		if (_frame > kAnimEndFrame * kDropNum)
		{
			_frame = 0;
			_state = HammerBossState::Idle;
			_nowAnim = _idleAnim;
		}
	}
	else if (_state == HammerBossState::Death)
	{
		_frame++;
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
	if (_state == HammerBossState::Death)
	{

		if (_frame < kDeathFrame)
		{
			if (_frame % kDeathFrickerFrame * 2 < kDeathFrickerFrame)
			{
				SetDrawBright(255, 0, 0);	// 赤く点滅させる
			}
			_nowAnim.Draw(drawPos, true);
			SetDrawBright(255, 255, 255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			if (_frame % kDeathFrickerFrame == 0)
			{
				Vector2 explosionPosGap;
				explosionPosGap.x = GetRand(kDeathExplosionGapDis * 2) - kDeathExplosionGapDis;
				explosionPosGap.y = GetRand(kDeathExplosionGapDis * 2) - kDeathExplosionGapDis;
				_pEffectManager->Create(GetColliderPos() + explosionPosGap, EffectType::ExplosionSmall);
			}
		}
		if (_frame == kDeathFrame)
		{
			_pEffectManager->Create(GetColliderPos(), EffectType::ExplosionBig);
		}
	}
	else
	{
		if (_damageFrame > 0)
		{
			SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
		}
		_nowAnim.Draw(drawPos, true);
		SetDrawBright(255, 255, 255);	// 明るさリセット
	}

#ifdef _DEBUG
	_pCollider->Draw(offset);
	_pAttackCollider->Draw(offset);
#endif
}

void HammerBoss::TakeDamage(int damage, Bullet& bullet)
{
	if (!(_hp <= 0))
	{
		if (bullet.GetType() == BulletType::NormalShot)
		{
			bullet.Reflect();
		}
		else if (bullet.GetType() == BulletType::ChargeShot)
		{
			_hp -= damage;
			_damageFrame = 5;
			bullet.Hit();
		}
		
		if (_hp <= 0)
		{
			_state = HammerBossState::Death;
			_frame = 0;
			_hp = 0;
			_pCamera->Shake(120, 5);
			_pCollider->SetIsEnabled(false);
			_pAttackCollider->SetIsEnabled(false);
			_nowAnim = _rapidAttackAnim;
		}
	}
}

bool HammerBoss::GetIsAlive() const
{
	return !(_state == HammerBossState::Death && _frame > kAliveTimeBeforeDeath);
}