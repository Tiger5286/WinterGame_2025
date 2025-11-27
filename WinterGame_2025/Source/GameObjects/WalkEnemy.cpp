#include "WalkEnemy.h"
#include "../Utility/BoxCollider.h"
#include "Dxlib.h"
#include "Bullet.h"
#include "Player.h"
#include "../Map.h"
#include "../Game.h"

namespace
{
	// 描画関係
	constexpr int kGraphCutW = 32;
	constexpr int kGraphCutH = 36;
	const Vector2 kFrameSize = { kGraphCutW,kGraphCutH };
	constexpr float kDrawScale = 3.0f;

	// 動き関連
	constexpr float kMoveSpeed = 4.0f;

	// 当たり判定
	constexpr float kColliderW = 80;
	constexpr float kColliderH = 80;
	const Vector2 kColliderSize = { kColliderW,kColliderH };

	constexpr float kGroundH = 800.0f;

	// アニメーション
	constexpr int kIdleAnimNum = 5;
	constexpr int kMoveAnimNum = 8;
	constexpr int kFallAnimNum = 1;

	constexpr int kOneAnimFrame = 6;

	// HP
	constexpr int kMaxHp = 5;
}

enum class WalkEnemyAnimType : int
{
	Idle = 0,
	Move,
	Fall
};

WalkEnemy::WalkEnemy(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, WalkEnemyState state, bool isTurn) :
	Enemy(kMaxHp, pPlayer),
	_handle(handle),
	_state(state),
	_isTurn(isTurn)
{
	_pos = ChipPosToGamePos(chipPos);
	_pos.y += GlobalConstants::kDrawChipSizeHalf;	// チップ半分下にずらす
	_pCollider = std::make_shared<BoxCollider>(_pos, kColliderSize);
}

WalkEnemy::~WalkEnemy()
{
}

void WalkEnemy::Init()
{
	_idleAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Idle), kFrameSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_moveAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Move), kFrameSize, kMoveAnimNum, kOneAnimFrame, kDrawScale);
	_fallAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Fall), kFrameSize, kFallAnimNum, kOneAnimFrame, kDrawScale);
	_nowAnim = _idleAnim;
}

void WalkEnemy::Update(Map& map)
{
	Gravity();

	if (_state == WalkEnemyState::Idle)
	{	// 待機状態の時はプレイヤーの方を向く
		_vel.x = 0.0f;
		float toPlayerDis = _pPlayer->GetPos().x - _pos.x;
		if (toPlayerDis < 0)
		{
			_isTurn = true;
		}
		else
		{
			_isTurn = false;
		}
	}
	else if (_state == WalkEnemyState::Move)
	{	// 行動状態の時は向いている方向に動く
		if (_isTurn)
		{
			_vel.x = -kMoveSpeed;
		}
		else
		{
			_vel.x = kMoveSpeed;
		}
	}

	// マップとの当たり判定
	HitDirection hitDir;
	hitDir = MapCollision(map);
	if (hitDir.right)	// 右に当たったら左向きにする
	{
		_isTurn = true;
	}
	if (hitDir.left)	// 左に当たったら右向きにする
	{
		_isTurn = false;
	}

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	UpdateAnim();
}

void WalkEnemy::Draw(Vector2 offset)
{
	//DrawRectRotaGraph(_pos.x, _pos.y - WALKENEMY_GRAPH_CUT_H / 2 * DRAW_SCALE, WALKENEMY_GRAPH_CUT_W * 0, WALKENEMY_GRAPH_CUT_H * 0, WALKENEMY_GRAPH_CUT_W, WALKENEMY_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true);
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - kGraphCutH / 2 * kDrawScale }, _isTurn);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}

void WalkEnemy::UpdateAnim()
{
	if (abs(_vel.y) > 0)
	{
		ChangeAnim(_fallAnim);
	}
	else if (abs(_vel.x) > 1.0f)
	{
		ChangeAnim(_moveAnim);
	}
	else
	{
		ChangeAnim(_idleAnim);
	}
	_nowAnim.Update();
}
