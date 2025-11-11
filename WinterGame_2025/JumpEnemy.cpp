#include "JumpEnemy.h"
#include "Player.h"
#include "Dxlib.h"
#include "BoxCollider.h"

namespace
{
	// 動き関連
	constexpr float JUMP_POWER_X = 12.5f;
	constexpr float JUMP_POWER_Y = 20.0f;
	constexpr int JUMP_INTERVAL = 120;

	// 描画関連
	constexpr int GRAPH_WIDTH = 80;
	constexpr int GRAPH_HEIGHT = 48;
	const Vector2 GRAPH_FRAME_SIZE = { 80.0f,48.0f };

	constexpr int IDLE_ANIM_NUM = 4;
	constexpr int ATTACK_ANIM_NUM = 21;
	constexpr int ATTACK_IMPACT_FRAME_NUM = 12;	// ハンマーを振り下ろし、地面にぶつけるアニメーションのフレーム番号
	constexpr int ONE_ANIM_FRAME = 6;

	constexpr float DRAW_SCALE = 3.0f;

	// 当たり判定
	constexpr int COLLIDER_W = 35 * 3;
	constexpr int COLLIDER_H = 40 * 3;

	// プレイヤーに攻撃しようとする距離
	constexpr float ATTACK_DIS = 600.0f;
}

JumpEnemy::JumpEnemy(Vector2 firstPos,std::shared_ptr<Player> pPlayer, int handle) :
	Enemy(firstPos,5, pPlayer),
	_handle(handle),
	_attackCooltime(0),
	_attackFrame(0),
	_isAttacking(false),
	_isTurn(false)
{
	_collider = std::make_shared<BoxCollider>(_pos, Vector2{ COLLIDER_W,COLLIDER_H });
	_idleAnim.Init(_handle, 0, GRAPH_FRAME_SIZE, IDLE_ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_attackAnim.Init(_handle, 1, GRAPH_FRAME_SIZE, ATTACK_ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_nowAnim = _idleAnim;
}

JumpEnemy::~JumpEnemy()
{
}

void JumpEnemy::Init()
{
}

void JumpEnemy::Update(Map& map)
{
	// プレイヤーとは逆の方を向く
	_pos.x < _pPlayer->GetPos().x ? _isTurn = true : _isTurn = false;

	if (_attackCooltime > 0)
	{
		_attackCooltime--;
	}

	// 前のフレームで攻撃行動をしていたかどうか持っておく
	bool isPrevAttacking = _isAttacking;
	// プレイヤーが攻撃範囲におり、かつ攻撃クールタイムが明けていたら攻撃行動に移る
	bool isCanAttackDis = abs(_pos.x - _pPlayer->GetPos().x) < ATTACK_DIS;	// プレイヤーが攻撃範囲にいる
	bool isCanAttackCoolTime = _attackCooltime == 0;	// クールタイムが明けている
	if (isCanAttackDis && isCanAttackCoolTime)
	{	// 条件を満たしたら攻撃中フラグを有効化
		_isAttacking = true;
	}

	Gravity();

	if (_isAttacking)	// 攻撃行動
	{	
		if (!isPrevAttacking)
		{	// 攻撃行動に移った最初のフレームだけ実行
			ChangeAnim(_attackAnim);
		}

		_attackFrame++;
		if (_attackFrame == ONE_ANIM_FRAME * ATTACK_IMPACT_FRAME_NUM)	// ハンマーを振り下ろすタイミング
		{	// プレイヤーの方向に跳ぶ
			_isTurn ? _vel.x = JUMP_POWER_X : _vel.x = -JUMP_POWER_X;
			_vel.y = -JUMP_POWER_Y;
			// マップに触れないように位置をずらす
			_isTurn ? _pos.x += 1.0f : _pos.x -= 1.0f; 
			_vel.y -= 1.0f;
		}
		if (_attackFrame == ONE_ANIM_FRAME * ATTACK_ANIM_NUM)	// 攻撃終了
		{
			_attackCooltime = 120;
			_attackFrame = 0;
			_isAttacking = false;
			ChangeAnim(_idleAnim);
		}
	}

	// マップに触れたら止まる
	if (MapCollision(map))
	{
		_vel.x = 0.0f;
	}

	// プレイヤーに当たったらダメージを与える
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void JumpEnemy::Draw(Vector2 offset)
{
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - GRAPH_HEIGHT / 2 * DRAW_SCALE }, _isTurn);
#ifdef _DEBUG
	// プレイヤーを検知する範囲を表示
	DrawLine(_pos.x - ATTACK_DIS - offset.x, 0, _pos.x - ATTACK_DIS - offset.x, 1080, 0x0000ff);
	DrawLine(_pos.x + ATTACK_DIS - offset.x, 0, _pos.x + ATTACK_DIS - offset.x, 1080, 0x0000ff);
	_collider->Draw(offset);
#endif
}