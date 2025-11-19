#include "Boss.h"
#include "Game.h"
#include "BoxCollider.h"
#include "Player.h"
#include "Dxlib.h"
#include "Camera.h"
#include "Gimmick.h"


namespace
{
	// 描画関係
	const Vector2 GRAPH_SIZE = { 32,36 };
	constexpr float DRAW_SCALE = 6.0f;
	// 当たり判定
	const Vector2 COLLIDER_SIZE = { 130,150 };
	// 初期位置
	const Vector2 firstChipPos = { 32,18 };
	// 体力
	constexpr int MAX_HP = 100;

	// 壁走り、天井は知り時の描画角度
	constexpr float ANGLE_90 = DX_PI_F / 2;	// 90度
	constexpr float ANGLE_180 = DX_PI_F;	// 180度
	constexpr float ANGLE_270 = DX_PI_F / 2 * 3;	// 270度

	// 角度が変わったときの描画オフセット
	const Vector2 ANGLE_90_OFFSET = { 45,35 };
	const Vector2 ANGLE_180_OFFSET = { 0,65 };
	const Vector2 ANGLE_270_OFFSET = { -45,35 };

	// 行動関係
	constexpr int RUN_READY_FRAME = 60;	// 突進準備のフレーム数
	constexpr int STUN_FRAME = 120;	// スタン時間
	constexpr float RUN_SPEED = 10.0f;	// 突進速度
	constexpr float WALL_RUN_SPEED = 7.5f;	// 壁走りの上昇速度
}

Boss::Boss(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Gimmick> pLaser, int handle) :
	Enemy(MAX_HP, pPlayer),
	_handle(handle),
	_isTurn(true),
	_frame(0),
	_state(BossState::Idle),
	_pCamera(pCamera),
	_pLaser(pLaser)
{
	_pos = ChipPosToGamePos(firstChipPos);
	_pos.y += GlobalConstants::DRAW_CHIP_SIZE_HALF;	// チップ半分下にずらす
	_collider = std::make_shared<BoxCollider>(_pos, COLLIDER_SIZE);
	_collider->SetPosToBox(_pos);

	_idleAnim.Init(_handle, 0, GRAPH_SIZE, 5, 6, DRAW_SCALE);
	_stunAnim.Init(_handle, 1, GRAPH_SIZE, 8, 10, DRAW_SCALE);
	_tackleAnim.Init(_handle, 1, GRAPH_SIZE, 8, 3, DRAW_SCALE);
	_fallAnim.Init(_handle, 2, GRAPH_SIZE, 1, 6, DRAW_SCALE);
	_nowAnim = _idleAnim;
}

Boss::~Boss()
{
}

void Boss::Init()
{
}

void Boss::Update(Map& map)
{
#ifdef _DEBUG

	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeState(BossState::Idle);
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		ChangeState(BossState::Tackle);
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		ChangeState(BossState::TackleAndWallRun);
	}
#endif // _DEBUG

	Gravity();

	// 待機状態の時の処理
	if (_state == BossState::Idle)
	{
		_frame++;
		ChangeAnim(_idleAnim);
		float toPlayerDis = _pPlayer->GetPos().x - _pos.x;
		toPlayerDis < 0 ? _isTurn = true : _isTurn = false;
		if (_frame == 60)
		{
			bool attackType = GetRand(1);
			attackType ? ChangeState(BossState::Tackle) : ChangeState(BossState::TackleAndWallRun);
		}
	}
	// スタン時の処理
	if (_state == BossState::Stun)
	{
		_frame++;
		ChangeAnim(_stunAnim);
		_vel = Vector2::Vlerp(_vel, Vector2(), 0.1f);
		if (_frame == STUN_FRAME)
		{
			ChangeState(BossState::Idle);
		}
	}
	//タックル攻撃の時の処理
	if (_state == BossState::Tackle)
	{
		_frame++;
		// 最初は走るモーションだけ
		if (_frame == 1)
		{
			_pLaser->SetPos(Vector2(_pos.x, 15 * 48 + 24));
			ChangeAnim(_tackleAnim);
		}
		_pLaser->SetPos(Vector2(_pos.x, _pLaser->GetPos().y));	// レーザーの位置を合わせる

		// 走り出す
		if (_frame == RUN_READY_FRAME)
		{
			_isTurn ? _vel.x = -RUN_SPEED : _vel.x = RUN_SPEED;
		}
		// どっちかの壁にぶつかったら
		if (_hitDir.right || _hitDir.left)
		{
			// 跳ね返ってスタン
			_vel.y = -10.0f;
			_isTurn ? _vel.x = 5.0f : _vel.x = -5.0f;
			ChangeState(BossState::Stun);
			_frame = 0;
			_pCamera->Shake(15, 5);
			_pLaser->SetPos(Vector2(-10,-10));	// レーザーの位置を消す(画面外に行くだけ)
		}
	}
	// 壁走り突進の時の処理
	if (_state == BossState::TackleAndWallRun)
	{
		_frame++;
		// 最初は走るモーションだけ
		if (_frame == 1)
		{
			_pLaser->SetPos(Vector2(_pos.x, 15 * 48 + 24));	// レーザーを出す
			ChangeAnim(_tackleAnim);
		}

		// 走り出す
		if (_frame == RUN_READY_FRAME)
		{
			_isTurn ? _vel.x = -RUN_SPEED : _vel.x = RUN_SPEED;
		}

		// 左の壁にぶつかったら
		if (_hitDir.left)
		{
			_pLaser->SetPos(Vector2(-10, -10));	// レーザーを消す(画面外に行くだけ)
			// 上に向かって走る
			_nowAnim.SetRotate(ANGLE_90);
			_nowAnim.SetOffset(ANGLE_90_OFFSET);
			_vel.x = -1.0f;
			_vel.y = -WALL_RUN_SPEED;
			if (_hitDir.up)
			{
				_nowAnim.SetRotate(DX_PI_F);
				_nowAnim.SetOffset(ANGLE_180_OFFSET);
				ChangeState(BossState::CeilingRun);
			}
		}
		else if (_hitDir.right) // 右の壁にぶつかったら
		{
			_pLaser->SetPos(Vector2(-10, -10));	// レーザーを消す(画面外に行くだけ)
			// 上に向かって走る
			_nowAnim.SetRotate(ANGLE_270);
			_nowAnim.SetOffset(ANGLE_270_OFFSET);
			_vel.x = 1.0f;
			_vel.y = -WALL_RUN_SPEED;
			if (_hitDir.up)
			{
				_nowAnim.SetRotate(DX_PI_F);
				_nowAnim.SetOffset(ANGLE_180_OFFSET);
				ChangeState(BossState::CeilingRun);
			}
		}
		else
		{
			// どっちの壁にも当たっていない時レーザーを出す
			_pLaser->SetPos(Vector2(_pos.x, _pLaser->GetPos().y));
		}
		
	}
	// 天井走りの時の処理
	if (_state == BossState::CeilingRun)
	{
		_vel.y = -1.0f;
		_isTurn ? _vel.x = RUN_SPEED : _vel.x = -RUN_SPEED;
		float toPlayerDis = _pPlayer->GetPos().x - _pos.x;
		if (abs(toPlayerDis) < 75.0f)
		{
			_vel.x = 0.0f;
			ChangeState(BossState::FallAttack);
		}
	}
	// 落下攻撃の時の処理
	if (_state == BossState::FallAttack)
	{
		ChangeAnim(_fallAnim);
		_nowAnim.SetRotate(0.0f);
		_nowAnim.SetOffset(Vector2());
		if (_hitDir.down)
		{
			_pCamera->Shake(15, 5);
			ChangeState(BossState::Idle);
		}
	}

	_hitDir = MapCollision(map);

	// プレイヤーに当たったらダメージを与える
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void Boss::Draw(Vector2 offset)
{
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - GRAPH_SIZE.y / 2 * DRAW_SCALE }, _isTurn);
#ifdef _DEBUG
	_collider->Draw(offset);
#endif
}

void Boss::ChangeState(BossState state)
{
	_state = state;
	_frame = 0;
}
