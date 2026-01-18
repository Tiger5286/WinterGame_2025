#include "WalkBoss.h"
#include "../../Game.h"
#include "../../Utility/BoxCollider.h"
#include "../Player.h"
#include "Dxlib.h"
#include "../../Systems/Camera.h"
#include "../Gimmick.h"
#include "../../Systems/EffectManager.h"
#include "../../Systems/SoundManager.h"
#include "../../Scenes/SceneManager.h"
#include "../../Scenes/SceneClear.h"
#include "../Bullet.h"

namespace
{
	// 描画関係
	const Vector2 kGraphSize = { 32,36 };
	constexpr float kDrawScale = 6.0f;
	// 当たり判定
	const Vector2 kColliderSize = { 130,150 };
	// 体力
	constexpr int kMaxHp = 60;
	constexpr int kScore = 10000;

	// アニメーション関連
	constexpr int kIdleAnimIndex = 0;
	constexpr int kIdleAnimNum = 5;
	constexpr int kRunAnimIndex = 1;
	constexpr int kRunAnimNum = 8;
	constexpr int kFallAnimIndex = 2;
	constexpr int kFallAnimNum = 1;

	constexpr int kOneAnimFrameIdle = 6;
	constexpr int kOneAnimFrameStun = 10;
	constexpr int kOneAnimFrameRun = 3;
	constexpr int kOneAnimFrameFall = 1;

	constexpr int kBarrierGraphSize = 16;
	constexpr int kBarrierAnimNum = 4;
	constexpr float kBarrierDrawScale = 15.0f;
	constexpr int kBarrierOneAnimFrame = 5;

	constexpr int kDrawBarrierMaxFrame = 15;	// バリアを描画する最大フレーム数

	// 壁走り、天井は知り時の描画角度
	constexpr float kAngle90 = DX_PI_F / 2;	// 90度
	constexpr float kAngle180 = DX_PI_F;	// 180度
	constexpr float kAngle270 = DX_PI_F / 2 * 3;	// 270度

	// 角度が変わったときの描画オフセット
	const Vector2 kAngle90Offset = { 45,35 };
	const Vector2 kAngle180Offset = { 0,65 };
	const Vector2 kAngle270Offset = { -45,35 };

	// 行動関係
	constexpr int kIdleFrame = 90;	// 待機時間

	constexpr int kStunFrame = 150;	// スタン時間
	constexpr float kStunVelY = 10.0f;
	constexpr float kStunVelX = 5.0f;

	constexpr int kRunReadyFrame = 60;	// 突進準備のフレーム数
	constexpr float kRunSpeed = 10.0f;	// 突進速度
	constexpr float kWallRunSpeed = 7.5f;	// 壁走りの上昇速度

	constexpr int kLaserPosY = 15 * GlobalConstants::kDrawChipSize + GlobalConstants::kDrawChipSizeHalf;

	// 演出関連
	constexpr int kCameraShakePower = 5;
	constexpr int kCameraShakeFrame = 15;
	constexpr int kDeathFrame = 120;
	constexpr int kDeathFrickerFrame = 10;
	constexpr int kDeathExplosionGapDis = 100;
	constexpr int kAliveTimeBeforeDeath = 210;
}

WalkBoss::WalkBoss(Vector2 firstPos,std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, std::shared_ptr<Camera> pCamera, std::shared_ptr<Gimmick> pLaser,
				   ItemManager& itemManager,SceneManager& sceneManager, int handle,int barrierH) :
	Enemy(kMaxHp, kScore, pPlayer,pEffectManager,itemManager,sceneManager),
	_handle(handle),
	_barrierH(barrierH),
	_isTurn(true),
	_frame(0),
	_state(WalkBossState::Idle),
	_pCamera(pCamera),
	_pLaser(pLaser)
{
	_pos = ChipPosToGamePos(firstPos);
	_pos.y += GlobalConstants::kDrawChipSizeHalf;	// チップ半分下にずらす
	_pCollider = std::make_shared<BoxCollider>(_pos, kColliderSize);
	_pCollider->SetPosToBox(_pos);

	_idleAnim.Init(_handle, kIdleAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrameIdle, kDrawScale);
	_stunAnim.Init(_handle, kRunAnimIndex, kGraphSize, kRunAnimNum, kOneAnimFrameStun, kDrawScale);
	_tackleAnim.Init(_handle, kRunAnimIndex, kGraphSize, kRunAnimNum, kOneAnimFrameRun, kDrawScale);
	_fallAnim.Init(_handle, kFallAnimIndex, kGraphSize, kFallAnimNum, kOneAnimFrameFall, kDrawScale);
	_nowAnim = _idleAnim;
	_barrierAnim.Init(_barrierH, 0, Vector2(kBarrierGraphSize, kBarrierGraphSize), kBarrierAnimNum, kBarrierOneAnimFrame, kBarrierDrawScale);
}

WalkBoss::~WalkBoss()
{
	_pLaser->SetPos(Vector2(-1, -1));	// レーザーの位置を消す(画面外に行くだけ)
}

void WalkBoss::Init()
{
}

void WalkBoss::Update(Map& map)
{
#ifdef _DEBUG

	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeState(WalkBossState::Idle);
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		ChangeState(WalkBossState::Tackle);
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		ChangeState(WalkBossState::TackleAndWallRun);
	}
	if (CheckHitKey(KEY_INPUT_0))
	{
		_hp = 1;
	}
#endif // _DEBUG

	BaseUpdate();

	Gravity();
	bool isEnableLaser = false;

	// 待機状態の時の処理
	if (_state == WalkBossState::Idle)
	{
		_frame++;
		ChangeAnim(_idleAnim);
		float toPlayerDis = _pPlayer->GetPos().x - _pos.x;
		toPlayerDis < 0 ? _isTurn = true : _isTurn = false;
		if (_frame == kIdleFrame)
		{
			bool attackType = GetRand(1);
			attackType ? ChangeState(WalkBossState::Tackle) : ChangeState(WalkBossState::TackleAndWallRun);
		}
	}
	// スタン時の処理
	if (_state == WalkBossState::Stun)
	{
		_frame++;
		ChangeAnim(_stunAnim);
		_vel = Vector2::Vlerp(_vel, Vector2(), 0.1f);
		if (_frame == kStunFrame)
		{
			ChangeState(WalkBossState::Idle);
		}
	}
	//タックル攻撃の時の処理
	if (_state == WalkBossState::Tackle)
	{
		_frame++;
		isEnableLaser = true;

		// 最初は走るモーションだけ
		ChangeAnim(_tackleAnim);

		// レーザーの起動音を鳴らす
		if (_frame == 1)
		{
			SoundManager::GetInstance().PlaySoundGame("LaserActive");
		}

		// 足音を鳴らす
		if (_frame % 10 == 0)
		{
			SoundManager::GetInstance().PlaySoundGame("WalkBossRun");
		}

		// 走り出す
		if (_frame == kRunReadyFrame)
		{
			_isTurn ? _vel.x = -kRunSpeed : _vel.x = kRunSpeed;
		}
		// どっちかの壁にぶつかったら
		if (_hitDir.right || _hitDir.left)
		{
			// 跳ね返ってスタン
			_vel.y = -kStunVelY;
			_isTurn ? _vel.x = kStunVelX : _vel.x = -kStunVelX;
			ChangeState(WalkBossState::Stun);
			_frame = 0;
			_pCamera->Shake(kCameraShakeFrame, kCameraShakePower);
			SoundManager::GetInstance().PlaySoundGame("Damage");
		}
	}
	// 壁走り突進の時の処理
	if (_state == WalkBossState::TackleAndWallRun)
	{
		_frame++;
		// 最初は走るモーションだけ
		if (_frame == 1)
		{
			ChangeAnim(_tackleAnim);
			SoundManager::GetInstance().PlaySoundGame("LaserActive");
		}

		// 足音を鳴らす
		if (_frame % 10 == 0)
		{
			SoundManager::GetInstance().PlaySoundGame("WalkBossRun");
		}

		// 走り出す
		if (_frame == kRunReadyFrame)
		{
			_isTurn ? _vel.x = -kRunSpeed : _vel.x = kRunSpeed;
		}

		// 左の壁にぶつかったら
		if (_hitDir.left)
		{
			// 上に向かって走る
			_nowAnim.SetRotate(kAngle90);
			_nowAnim.SetOffset(kAngle90Offset);
			_vel.x = -1.0f;	// X速度を壁に張り付くようにする
			_vel.y = -kWallRunSpeed;
			if (_hitDir.up)
			{
				_nowAnim.SetRotate(DX_PI_F);
				_nowAnim.SetOffset(kAngle180Offset);
				ChangeState(WalkBossState::CeilingRun);
			}
		}
		else if (_hitDir.right) // 右の壁にぶつかったら
		{
			// 上に向かって走る
			_nowAnim.SetRotate(kAngle270);
			_nowAnim.SetOffset(kAngle270Offset);
			_vel.x = 1.0f;	// X速度を壁に張り付くようにする
			_vel.y = -kWallRunSpeed;
			if (_hitDir.up)
			{
				_nowAnim.SetRotate(DX_PI_F);
				_nowAnim.SetOffset(kAngle180Offset);
				ChangeState(WalkBossState::CeilingRun);
			}
		}
		else
		{
			// どっちの壁にも当たっていない時レーザーを出す
			isEnableLaser = true;
		}
		
	}
	// 天井走りの時の処理
	if (_state == WalkBossState::CeilingRun)
	{
		_frame++;

		// 足音を鳴らす
		if (_frame % 10 == 0)
		{
			SoundManager::GetInstance().PlaySoundGame("WalkBossRun");
		}

		_vel.y = -1.0f;	// 天井に張り付くようにY速度を上向きにする
		_isTurn ? _vel.x = kRunSpeed : _vel.x = -kRunSpeed;
		float toPlayerDis = _pPlayer->GetPos().x - _pos.x;
		if (abs(toPlayerDis) < 75.0f)
		{
			_vel.x = 0.0f;
			ChangeState(WalkBossState::FallAttack);
		}
	}
	// 落下攻撃の時の処理
	if (_state == WalkBossState::FallAttack)
	{
		ChangeAnim(_fallAnim);
		_nowAnim.SetRotate(0.0f);
		_nowAnim.SetOffset(Vector2());
		if (_hitDir.down)
		{
			_pCamera->Shake(kCameraShakeFrame, kCameraShakePower);
			ChangeState(WalkBossState::Idle);
			SoundManager::GetInstance().PlaySoundGame("FallLanding");
		}
	}
	// 死亡時の処理
	if (_state == WalkBossState::Death)
	{
		_frame++;
		_pCollider->SetIsEnabled(false);
		ChangeAnim(_tackleAnim);
	}

	_hitDir = MapCollision(map);

	if (isEnableLaser)
	{
		_pLaser->SetPos(Vector2(_pos.x, kLaserPosY));	// レーザーの位置を合わせる
	}
	else
	{
		_pLaser->SetPos(Vector2(-1, -1));	// レーザーの位置を消す(画面外に行くだけ)
	}

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
	_barrierAnim.Update();
}

void WalkBoss::Draw(Vector2 offset)
{
	BaseDraw(offset);
	if (_state == WalkBossState::Death)	// 死亡時の演出描画
	{
		if (_frame < kDeathFrame)
		{
			if (_frame % kDeathFrickerFrame * 2 < kDeathFrickerFrame)
			{
				SetDrawBright(255, 0, 0);	// 赤く点滅させる
			}
			_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - kGraphSize.y / 2 * kDrawScale }, _isTurn);
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
		_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - kGraphSize.y / 2 * kDrawScale }, _isTurn);
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
#endif
}

bool WalkBoss::GetIsAlive() const
{
	return !(_state == WalkBossState::Death && _frame > kAliveTimeBeforeDeath);
}

void WalkBoss::TakeDamage(int damage,Bullet& bullet)
{
	if (!(_hp <= 0))
	{
		if (_state == WalkBossState::Tackle ||		// 攻撃行動中は通常弾のダメージを受けない
			_state == WalkBossState::TackleAndWallRun ||
			_state == WalkBossState::CeilingRun ||
			_state == WalkBossState::FallAttack)
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
		else
		{
			_hp -= damage;
			_damageFrame = 5;
			SoundManager::GetInstance().PlaySoundGame("EnemyDamage");
			bullet.Hit();
		}
		if (_hp <= 0)
		{
			_state = WalkBossState::Death;
			_frame = 0;
			_hp = 0;
			_pCamera->Shake(kDeathFrame, kCameraShakePower);
			_vel = Vector2();
		}
	}
}

void WalkBoss::ChangeState(WalkBossState state)
{
	_state = state;
	_frame = 0;
}