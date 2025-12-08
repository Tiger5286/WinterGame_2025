#include "FlyBoss.h"
#include "Dxlib.h"
#include "../../Utility/CircleCollider.h"
#include "../../Systems/Camera.h"
#include "../../Systems/EnemyManager.h"
#include "../../Systems/EffectManager.h"
#include "../../Systems/BulletManager.h"
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

	// 当たり判定
	constexpr int kColliderRadius = 70;

	// 動きの制御関連
	// 縦揺れ関連
	constexpr float kSinAngleIncrement = 0.03;
	constexpr int kSinAngleScale = 40;
	// Idle状態関連
	constexpr int kIdleWaitFrame = 60;
	// Tackle状態関連
	constexpr int kAimFrame = 120;
	constexpr float kAimLerpRate = 0.05f;
	constexpr float kRunupVel = 15.0f;		// タックル開始時の後ろに下がる時の速度
	constexpr float kTackleAccel = 1.0f;	// タックル中の加速度
	constexpr float kMaxTackleVel = 40.0f;	// タックル中の最大速度
	// Back状態関連
	constexpr int kBasePosXRight = 1600;
	constexpr int kBasePosXLeft = 300;
	constexpr int kBasePosY = 550;
	constexpr float kBackSpeed = 5.0f;
	constexpr int kBackWaitFrame = 30;
	constexpr float kBackDisThreshold = 5.0f;

	constexpr int kOutOfScreenMargin = 100;

	constexpr int kHp = 100;
	constexpr int kScore = 10000;
}

FlyBoss::FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, EnemyManager& enemyManager, BulletManager& bulletManager, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle) :
	Enemy(kHp, kScore, pPlayer, pEffectManager, sceneManager),
	_handle(handle),
	_pCamera(pCamera),
	_enemyManager(enemyManager),
	_bulletManager(bulletManager)
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
			_state = FlyBossState::FallBall;
			// 敵が複数いる、もしくは前回の行動が召喚なら他の攻撃、そうでなければドローン召喚へ
			//if (_enemyManager.GetEnemies().size() > 1 ||
			//	_prevState == FlyBossState::SummonEnemies)
			//{
			//	_state = FlyBossState::FallBall;
			//}
			//else
			//{
			//	_state = FlyBossState::SummonEnemies;
			//}
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
	else if (_state == FlyBossState::SummonEnemies)
	{
		// 一定間隔でドローンを召喚
		if (_frame % 120 == 0)
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
		_pos += vec * 5.0f;
		if ((targetPos - _pos).Length() < 5.0f)
		{
			_state = FlyBossState::Idle;
			_prevState = FlyBossState::SummonEnemies;
			_frame = 0;
		}
	}
	else if (_state == FlyBossState::FallBall)
	{
		_frame++;
		_pos.x = std::lerp(_pos.x, _pPlayer->GetColliderPos().x, kAimLerpRate);
		_pos.y = std::lerp(_pos.y, _pPlayer->GetColliderPos().y - 400, kAimLerpRate);
		if (_pos.y < 100)
		{
			_pos.y = 100;
		}
		if (_frame % 60 == 0)
		{
			Vector2 shotPos = _pos + Vector2(0, 50);
			_bulletManager.ShotFallBall(shotPos);
		}

		if (_frame > 60 * 6)
		{
			_state = FlyBossState::Back;
			_frame = 0;
		}
	}

	_pos += _vel;

	// 縦揺れの位置を計算
	_sinAngle += kSinAngleIncrement;
	Vector2 adjustPos = _pos;
	adjustPos.y = _pos.y + sinf(_sinAngle) * kSinAngleScale;

	_pCollider->SetPos(adjustPos);

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
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
	if (_damageFrame > 0)
	{
		SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
	}
	// コライダーが縦揺れを適用した位置にいるのでそのまま使う
	_nowAnim.Draw(_pCollider->GetPos() - offset, false);
	SetDrawBright(255, 255, 255);	// 明るさリセット
#ifdef _DEBUG
	_pCollider->Draw(offset);
	DrawFormatString(_pos.x, _pos.y - 60, 0xffffff, "_frame:%d", _frame);
	//DrawFormatString(_pos.x, _pos.y - 80, 0xffffff, "_vel.x:%.2f", _vel.x);
#endif
}
