#include "WalkBoss.h"
#include "../../Game.h"
#include "../../Utility/BoxCollider.h"
#include "../Player.h"
#include "Dxlib.h"
#include "../../Systems/Camera.h"
#include "../Gimmick.h"


namespace
{
	// 描画関係
	const Vector2 kGraphSize = { 32,36 };
	constexpr float kDrawScale = 6.0f;
	// 当たり判定
	const Vector2 kColliderSize = { 130,150 };
	// 体力
	constexpr int kMaxHp = 100;

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

	// 壁走り、天井は知り時の描画角度
	constexpr float kAngle90 = DX_PI_F / 2;	// 90度
	constexpr float kAngle180 = DX_PI_F;	// 180度
	constexpr float kAngle270 = DX_PI_F / 2 * 3;	// 270度

	// 角度が変わったときの描画オフセット
	const Vector2 kAngle90Offset = { 45,35 };
	const Vector2 kAngle180Offset = { 0,65 };
	const Vector2 kAngle270Offset = { -45,35 };

	// 行動関係
	constexpr int kIdleFrame = 60;	// 待機時間

	constexpr int kStunFrame = 120;	// スタン時間
	constexpr float kStunVelY = 10.0f;
	constexpr float kStunVelX = 5.0f;

	constexpr int kRunReadyFrame = 60;	// 突進準備のフレーム数
	constexpr float kRunSpeed = 10.0f;	// 突進速度
	constexpr float kWallRunSpeed = 7.5f;	// 壁走りの上昇速度

	constexpr int kLaserPosY = 15 * GlobalConstants::kDrawChipSize + GlobalConstants::kDrawChipSizeHalf;

	// 演出関連
	constexpr int kCameraShakePower = 5;
	constexpr int kCameraShakeFrame = 15;
}

WalkBoss::WalkBoss(Vector2 firstPos,std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, std::shared_ptr<Camera> pCamera, std::shared_ptr<Gimmick> pLaser, int handle) :
	Enemy(kMaxHp, pPlayer,pEffectManager),
	_handle(handle),
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
		}
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
}

void WalkBoss::Draw(Vector2 offset)
{
	if (_damageFrame > 0)
	{
		SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
	}
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - kGraphSize.y / 2 * kDrawScale }, _isTurn);
	SetDrawBright(255, 255, 255);	// 明るさリセット
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}

void WalkBoss::ChangeState(WalkBossState state)
{
	_state = state;
	_frame = 0;
}
