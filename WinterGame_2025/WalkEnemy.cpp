#include "WalkEnemy.h"
#include "BoxCollider.h"
#include "Dxlib.h"
#include "Bullet.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"

namespace
{
	// 描画関係
	constexpr int WALKENEMY_GRAPH_CUT_W = 32;
	constexpr int WALKENEMY_GRAPH_CUT_H = 36;
	const Vector2 WALKENEMY_FRAME_SIZE = { WALKENEMY_GRAPH_CUT_W,WALKENEMY_GRAPH_CUT_H };
	constexpr float DRAW_SCALE = 3.0f;

	// 動き関連
	constexpr float MOVE_SPEED = 4.0f;

	// 当たり判定
	constexpr float COLLIDER_W = 80;
	constexpr float COLLIDER_H = 80;
	const Vector2 COLLIDER_SIZE = { COLLIDER_W,COLLIDER_H };

	constexpr float GROUND_H = 800.0f;

	// アニメーション
	constexpr int WALKENEMY_IDLE_ANIM_MAX_NUM = 5;
	constexpr int WALKENEMY_MOVE_ANIM_MAX_NUM = 8;
	constexpr int WALKENEMY_FALL_ANIM_MAX_NUM = 1;

	constexpr int WALKENEMY_ONE_ANIM_FRAME = 6;

	// HP
	constexpr int MAX_HP = 5;
}

enum class WalkEnemyAnimType : int
{
	Idle = 0,
	Move,
	Fall
};

//WalkEnemy::WalkEnemy(WalkEnemyState state,bool isTurn,int handle, std::shared_ptr<Player>pPlayer):
//	Enemy(MAX_HP,pPlayer),
//	_handle(handle),
//	_isHitChargeShot(false),
//	_isTurn(isTurn),
//	_state(state)
//{
//	_collider = std::make_shared<BoxCollider>(_pos, Vector2{ COLLIDER_W,COLLIDER_H });
//}

//WalkEnemy::WalkEnemy(Vector2 firstPos, WalkEnemyState state, bool isTurn, int handle, std::shared_ptr<Player> pPlayer) :
//	Enemy(MAX_HP, pPlayer),
//	_handle(handle),
//	_isHitChargeShot(false),
//	_isTurn(isTurn),
//	_state(state)
//{
//	_collider = std::make_shared<BoxCollider>(_pos, Vector2{ COLLIDER_W,COLLIDER_H });
//	_pos = MapChipPosToGamePos(firstPos);
//}

WalkEnemy::WalkEnemy(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, WalkEnemyState state, bool isTurn) :
	Enemy(MAX_HP, pPlayer),
	_handle(handle),
	_state(state),
	_isTurn(isTurn)
{
	_pos = MapChipPosToGamePos(chipPos);
	_pos.y += GlobalConstants::DRAW_CHIP_SIZE_HALF;	// チップ半分下にずらす
	_collider = std::make_shared<BoxCollider>(_pos, COLLIDER_SIZE);
}

WalkEnemy::~WalkEnemy()
{
}

void WalkEnemy::Init()
{
	_idleAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Idle), WALKENEMY_FRAME_SIZE, WALKENEMY_IDLE_ANIM_MAX_NUM, WALKENEMY_ONE_ANIM_FRAME, DRAW_SCALE);
	_moveAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Move), WALKENEMY_FRAME_SIZE, WALKENEMY_MOVE_ANIM_MAX_NUM, WALKENEMY_ONE_ANIM_FRAME, DRAW_SCALE);
	_fallAnim.Init(_handle, static_cast<int>(WalkEnemyAnimType::Fall), WALKENEMY_FRAME_SIZE, WALKENEMY_FALL_ANIM_MAX_NUM, WALKENEMY_ONE_ANIM_FRAME, DRAW_SCALE);
	_nowAnim = _idleAnim;
}

void WalkEnemy::Update(Map& map)
{
	Gravity();

	if (_state == WalkEnemyState::Idle)
	{
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
	{
		if (_isTurn)
		{
			_vel.x = -MOVE_SPEED;
		}
		else
		{
			_vel.x = MOVE_SPEED;
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
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	UpdateAnim();
}

void WalkEnemy::Draw(Vector2 offset)
{
	//DrawRectRotaGraph(_pos.x, _pos.y - WALKENEMY_GRAPH_CUT_H / 2 * DRAW_SCALE, WALKENEMY_GRAPH_CUT_W * 0, WALKENEMY_GRAPH_CUT_H * 0, WALKENEMY_GRAPH_CUT_W, WALKENEMY_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _handle, true);
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - WALKENEMY_GRAPH_CUT_H / 2 * DRAW_SCALE }, _isTurn);
#ifdef _DEBUG
	_collider->Draw(offset);
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
