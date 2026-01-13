#include "FlyBoss.h"
#include "Dxlib.h"
#include "../../Utility/CircleCollider.h"
#include "../../Systems/Camera.h"
#include "../../Systems/EnemyManager.h"
#include "../../Systems/EffectManager.h"
#include "../../Systems/BulletManager.h"
#include "../../Systems/SoundManager.h"
#include "DroneEnemy.h"
#include "../../Game.h"
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

	constexpr int kBarrierGraphSize = 16;
	constexpr int kBarrierAnimNum = 4;
	constexpr float kBarrierDrawScale = 15.0f;
	constexpr int kBarrierOneAnimFrame = 5;

	constexpr int kDrawBarrierMaxFrame = 15;	// バリアを描画する最大フレーム数

	// 当たり判定
	constexpr int kColliderRadius = 70;

	// 動きの制御関連
	// 縦揺れ関連
	constexpr float kSinAngleIncrement = 0.03f;
	constexpr int kSinAngleScale = 40;
	// Idle状態関連
	constexpr int kIdleWaitFrame = 60;
	// Tackle状態関連
	constexpr int kAimFrame = 120;
	constexpr float kAimLerpRate = 0.05f;
	constexpr float kRunupVel = 15.0f;		// タックル開始時の後ろに下がる時の速度
	constexpr float kTackleAccel = 1.0f;	// タックル中の加速度
	constexpr float kMaxTackleVel = 40.0f;	// タックル中の最大速度
	// FallBall状態関連
	constexpr float kFollowPlayerOffsetY = 400;
	constexpr float kMaxPosY = 100;
	constexpr int kFallBallInterval = 60;
	constexpr int kMaxFallBallCount = 6;
	constexpr int kShotPosOffsetY = 50;
	// SummonEnemies状態関連
	constexpr int kSummonInterval = 120;
	constexpr float kMoveSpeed = 5.0f;
	// Back状態関連
	constexpr int kBasePosXRight = 1600;
	constexpr int kBasePosXLeft = 300;
	constexpr int kBasePosY = 550;
	constexpr float kBackSpeed = 5.0f;
	constexpr int kBackWaitFrame = 30;
	constexpr float kBackDisThreshold = 5.0f;

	constexpr int kOutOfScreenMargin = 100;

	constexpr int kHp = 30;
	constexpr int kScore = 10000;

	// 演出関連
	constexpr int kDeathFrame = 120;
	constexpr int kDeathFrickerFrame = 10;
	constexpr int kDeathExplosionGapDis = 100;
	constexpr int kAliveTimeBeforeDeath = 210;
}

FlyBoss::FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, EnemyManager& enemyManager, BulletManager& bulletManager, std::shared_ptr<EffectManager> pEffectManager, ItemManager& itemManager, SceneManager& sceneManager, int handle,int barrierH) :
	Enemy(kHp, kScore, pPlayer, pEffectManager, itemManager,sceneManager),
	_handle(handle),
	_barrierH(barrierH),
	_pCamera(pCamera),
	_enemyManager(enemyManager),
	_bulletManager(bulletManager)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale);
	_pos = ChipPosToGamePos(pos);
	_pCollider = std::make_shared<CircleCollider>(_pos, static_cast<float>(kColliderRadius));

	_barrierAnim.Init(_barrierH, 0, Vector2(kBarrierGraphSize, kBarrierGraphSize), kBarrierAnimNum, kBarrierOneAnimFrame, kBarrierDrawScale);
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
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_0))
	{
		_hp = 1;
	}
#endif
	if (_state == FlyBossState::AimAndTackle)
	{
		// 横位置が画面外に出たら
		if (_pos.x < _pCamera->GetPos().x - GlobalConstants::kScreenWidth / 2 - kOutOfScreenMargin ||
			_pos.x > _pCamera->GetPos().x + GlobalConstants::kScreenWidth / 2 + kOutOfScreenMargin)
		{
			_state = FlyBossState::Back;
			_prevState = FlyBossState::AimAndTackle;
			_frame = 0;
			_vel = Vector2(0, 0);
			CheckIsOnLeft();
		}
	}

	if (_state == FlyBossState::Back)
	{
		_frame++;
		// 少し間をおいてから元の位置に戻る
		if (_frame > kBackWaitFrame)
		{
			if (!_isOnLeft)
			{
				// 右の既定の位置に戻る
				Vector2 targetPos = Vector2(kBasePosXRight, kBasePosY);
				Vector2 vec = targetPos - _pos;
				vec.Normalize();
				_pos += vec * kBackSpeed;
				// 既定の位置に戻ったらIdle状態へ
				if (abs((_pos - targetPos).Length()) < kBackDisThreshold)
				{
					_state = FlyBossState::Idle;
					_frame = 0;
					_vel = Vector2(0, 0);
				}
			}
			else
			{
				// 左の既定の位置に戻る
				Vector2 targetPos = Vector2(kBasePosXLeft, kBasePosY);
				Vector2 vec = targetPos - _pos;
				vec.Normalize();
				_pos += vec * kBackSpeed;
				// 既定の位置に戻ったらIdle状態へ
				if (abs((_pos - targetPos).Length()) < kBackDisThreshold)
				{
					_state = FlyBossState::Idle;
					_frame = 0;
					_vel = Vector2(0, 0);
				}
			}
		}
	}
}

void FlyBoss::Update()
{
	BaseUpdate();

	if (_state == FlyBossState::Idle)	// Idle状態
	{
		_frame++;
		if (_frame > kIdleWaitFrame)
		{
			// 敵が複数いる、もしくは前回の行動が召喚なら召喚以外のランダムな攻撃、そうでなければ召喚を含めたランダムな攻撃を行う
			if (_enemyManager.GetEnemies().size() > 1 ||
				_prevState == FlyBossState::SummonEnemies)
			{
				auto nextState = static_cast<FlyBossState>(GetRand(1) + 1); // 1~3のランダム
				_state = nextState;
			}
			else
			{
				auto nextState = static_cast<FlyBossState>(GetRand(2) + 1); // 1~2のランダム
				_state = nextState;
			}
			_frame = 0;
		}
	}
	else if (_state == FlyBossState::AimAndTackle)	// Tackle状態
	{
		_frame++;
		if (_frame < kAimFrame)
		{
			_pos.y = std::lerp(_pos.y, _pPlayer->GetColliderPos().y, kAimLerpRate);
		}
		else if (_frame == kAimFrame)
		{
			CheckIsPlayerOnLeft();
			_isPlayerOnLeft ? _vel.x = kRunupVel : _vel.x = -kRunupVel;
		}
		else
		{
			// プレイヤーのいる方向に加速
			_isPlayerOnLeft ? _vel.x -= kTackleAccel : _vel.x += kTackleAccel;
			// 速度制限
			if (_vel.x > kMaxTackleVel)
			{
				_vel.x = kMaxTackleVel;
			}
			else if (_vel.x < -kMaxTackleVel)
			{
				_vel.x = -kMaxTackleVel;
			}
		}
	}
	else if (_state == FlyBossState::FallBall)
	{
		_frame++;
		// プレイヤーの真上に位置取る
		_pos.x = std::lerp(_pos.x, _pPlayer->GetColliderPos().x, kAimLerpRate);
		_pos.y = std::lerp(_pos.y, _pPlayer->GetColliderPos().y - kFollowPlayerOffsetY, kAimLerpRate);
		if (_pos.y < kMaxPosY)
		{
			_pos.y = kMaxPosY;
		}
		if (_frame % kFallBallInterval == 0)
		{
			Vector2 shotPos = _pos + Vector2(0, kShotPosOffsetY);
			_bulletManager.ShotFallBall(shotPos);
		}

		if (_frame > kFallBallInterval * kMaxFallBallCount)	// kMaxFallBallCount回ボールを落としたらステート変更
		{
			CheckIsOnLeft();
			_state = FlyBossState::Back;
			_frame = 0;
		}
	}
	else if (_state == FlyBossState::SummonEnemies)
	{
		// 一定間隔でドローンを召喚
		if (_frame % kSummonInterval == 0)
		{
			_pEffectManager->Create(_pos, EffectType::ItemGet);
			_enemyManager.Create(ObjectData::DroneEnemy, _pos, false);
		}

		_frame++;

		if (_frame == 1)
		{	// 最初に自分がどちら側にいるかを判定しておく
			CheckIsOnLeft();
		}

		Vector2 targetPos;
		if (_isOnLeft)
		{
			targetPos = { kBasePosXRight, kBasePosY };
		}
		else
		{
			targetPos = { kBasePosXLeft, kBasePosY };
		}
		Vector2 vec = targetPos - _pos;
		vec.Normalize();
		_pos += vec * kMoveSpeed;
		if ((targetPos - _pos).Length() < kMoveSpeed)
		{
			_state = FlyBossState::Idle;
			_prevState = FlyBossState::SummonEnemies;
			_frame = 0;
		}
	}
	else if (_state == FlyBossState::Death)
	{
		_frame++;
	}

	_pos += _vel;

	// 縦揺れの位置を計算	// 死んでいるなら揺れる速度を早くする
	_state == FlyBossState::Death ? _sinAngle += kSinAngleIncrement * 5 : _sinAngle += kSinAngleIncrement;
	Vector2 adjustPos = _pos;
	adjustPos.y = _pos.y + sinf(_sinAngle) * kSinAngleScale;

	_pCollider->SetPos(adjustPos);

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
	_barrierAnim.Update();
}

void FlyBoss::CheckIsPlayerOnLeft()
{
	_isPlayerOnLeft = _pos.x - _pPlayer->GetPos().x > 0;
}

void FlyBoss::CheckIsOnLeft()
{
	_isOnLeft = _pos.x < _pCamera->GetPos().x;
}

void FlyBoss::Draw(Vector2 offset)
{
	BaseDraw(offset);

	if (_state == FlyBossState::Death)	// 死亡時の演出描画
	{
		if (_frame < kDeathFrame)
		{
			if (_frame % kDeathFrickerFrame * 2 < kDeathFrickerFrame)
			{
				SetDrawBright(255, 0, 0);	// 赤く点滅させる
			}
			_nowAnim.Draw(GetColliderPos() - offset, false);
			SetDrawBright(255, 255, 255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			if (_frame % kDeathFrickerFrame == 0)
			{
				Vector2 explosionPosGap;
				explosionPosGap.x = static_cast<float>(GetRand(kDeathExplosionGapDis * 2) - kDeathExplosionGapDis);
				explosionPosGap.y = static_cast<float>(GetRand(kDeathExplosionGapDis * 2) - kDeathExplosionGapDis);
				_pEffectManager->Create(GetColliderPos() + explosionPosGap, EffectType::ExplosionSmall);
			}
		}
		if (_frame == kDeathFrame)
		{
			_pEffectManager->Create(GetColliderPos(), EffectType::ExplosionBig);
		}
	}
	else	// 通常描画
	{
		if (_damageFrame > 0)
		{
			SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
		}
		// コライダーが縦揺れを適用した位置にいるのでそのまま使う
		_nowAnim.Draw(_pCollider->GetPos() - offset, false);
		SetDrawBright(255, 255, 255);	// 明るさリセット

		// バリア描画
		if (_drawBarrierFrame > 0)
		{
			_drawBarrierFrame--;
		}
		float progress = static_cast<float>(_drawBarrierFrame) / kDrawBarrierMaxFrame;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(progress * 128));
		_barrierAnim.Draw(GetColliderPos() - offset, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
#ifdef _DEBUG
	_pCollider->Draw(offset);
	//DrawFormatString(_pos.x, _pos.y - 60, 0xffffff, "_frame:%d", _frame);
	//DrawFormatString(_pos.x, _pos.y - 80, 0xffffff, "_vel.x:%.2f", _vel.x);
#endif
}

void FlyBoss::TakeDamage(int damage,Bullet& bullet)
{
	if (!(_hp <= 0))
	{
		// タックル中なら通常弾を反射
		if (_state == FlyBossState::AimAndTackle)
		{
			if (bullet.GetType() == BulletType::NormalShot)
			{
				_drawBarrierFrame = kDrawBarrierMaxFrame;
				SoundManager::GetInstance().PlaySoundGame("Barrier");
				bullet.Reflect();
			}
			else if (bullet.GetType() == BulletType::ChargeShot)
			{
				_hp -= damage;
				_damageFrame = 5;
				SoundManager::GetInstance().PlaySoundGame("EnemyDamage");
				bullet.Hit();
			}
		}
		else	// それ以外の状態なら通常通りダメージを受ける
		{
			_hp -= damage;
			_damageFrame = 5;
			SoundManager::GetInstance().PlaySoundGame("EnemyDamage");
			// 通常弾なら消す
			if (bullet.GetType() == BulletType::NormalShot)
			{
				bullet.Hit();
			}
		}
		// HPが0以下になったら死亡処理
		if (_hp <= 0)
		{
			_state = FlyBossState::Death;
			_frame = 0;
			_hp = 0;
			_pCamera->Shake(120, 5);
			_vel = {};
			_pCollider->SetIsEnabled(false);

			// 自身が死んだとき、他に存在しているボス以外の敵がいたら全て倒す
			for (auto& enemy : _enemyManager.GetEnemies())
			{
				if (enemy->GetEnemyType() == EnemyType::Normal)
				{
					enemy->TakeDamage(9999, bullet);
				}
			}
		}
	}
}

bool FlyBoss::GetIsAlive() const
{
	return !(_state == FlyBossState::Death && _frame > kAliveTimeBeforeDeath);
}