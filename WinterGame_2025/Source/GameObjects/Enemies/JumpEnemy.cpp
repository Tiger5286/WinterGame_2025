#include "JumpEnemy.h"
#include "../Player.h"
#include "Dxlib.h"
#include "../../Utility/BoxCollider.h"
#include "../../Game.h"

namespace
{
	// 動き関連
	constexpr float kJumpPowerX = 12.5f;
	constexpr float kJumpPowerY = 20.0f;
	constexpr int kJumpInterval = 120;

	// 描画関連
	constexpr int kGraphWidth = 80;
	constexpr int kGraphHeight = 48;
	const Vector2 kGraphFrameSize = { 80.0f,48.0f };

	// アニメーション関連
	constexpr int kIdleAnimIndex = 0;
	constexpr int kAttackAnimIndex = 1;
	constexpr int kIdleAnimNum = 4;
	constexpr int kAttackAnimNum = 21;
	constexpr int kAttackImpactFrameNum = 12;	// ハンマーを振り下ろし、地面にぶつけるアニメーションのフレーム番号
	constexpr int kOneAnimFrame = 6;

	constexpr float kDrawScale = 3.0f;

	// 当たり判定
	constexpr int kColliderW = 35 * 3;
	constexpr int kColliderH = 32.5f * 3;

	// プレイヤーに攻撃しようとする距離
	constexpr float kAttackDis = 600.0f;

	// hp
	constexpr int kHp = 10;

	// スコア
	constexpr int kScore = 300;
}

JumpEnemy::JumpEnemy(Vector2 firstPos,std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle) :
	Enemy(kHp, kScore, pPlayer,pEffectManager,sceneManager),
	_handle(handle),
	_attackCooltime(0),
	_attackFrame(0),
	_isAttacking(false),
	_isTurn(false)
{
	_pCollider = std::make_shared<BoxCollider>(_pos, Vector2{ kColliderW,kColliderH });
	_pos = ChipPosToGamePos(firstPos);
	_pos.y += GlobalConstants::kDrawChipSizeHalf;

	_idleAnim.Init(_handle, kIdleAnimIndex, kGraphFrameSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_attackAnim.Init(_handle, kAttackAnimIndex, kGraphFrameSize, kAttackAnimNum, kOneAnimFrame, kDrawScale);
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
	BaseUpdate();

	// プレイヤーとは逆の方を向く
	if (!_isAttacking)	// 攻撃中は向きを変えない
	{
		_pos.x < _pPlayer->GetPos().x ? _isTurn = true : _isTurn = false;
	}
	

	if (_attackCooltime > 0)
	{
		_attackCooltime--;
	}

	// 前のフレームで攻撃行動をしていたかどうか持っておく
	bool isPrevAttacking = _isAttacking;
	// プレイヤーが攻撃範囲におり、かつ攻撃クールタイムが明けていたら攻撃行動に移る
	bool isCanAttackDis = abs(_pos.x - _pPlayer->GetPos().x) < kAttackDis;	// プレイヤーが攻撃範囲にいる
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
		if (_attackFrame == kOneAnimFrame * kAttackImpactFrameNum)	// ハンマーを振り下ろすタイミング
		{	// プレイヤーの方向に跳ぶ
			_isTurn ? _vel.x = kJumpPowerX : _vel.x = -kJumpPowerX;
			_vel.y = -kJumpPowerY;
			// マップに触れないように位置をずらす
			_isTurn ? _pos.x += 1.0f : _pos.x -= 1.0f; 
			_vel.y -= 1.0f;
		}
		if (_attackFrame == kOneAnimFrame * kAttackAnimNum)	// 攻撃終了
		{
			_attackCooltime = 120;
			_attackFrame = 0;
			_isAttacking = false;
			ChangeAnim(_idleAnim);
		}
	}

	// 地面に触れたら横移動を止める
	if (MapCollision(map).down)
	{
		_vel.x = 0.0f;
	}

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void JumpEnemy::Draw(Vector2 offset)
{
	if (_damageFrame > 0)
	{
		SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
	}
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - kGraphHeight / 2 * kDrawScale }, _isTurn);
	SetDrawBright(255, 255, 255);	// 明るさリセット
#ifdef _DEBUG
	// プレイヤーを検知する範囲を表示
	DrawLine(_pos.x - kAttackDis - offset.x, 0, _pos.x - kAttackDis - offset.x, 1080, 0x0000ff);
	DrawLine(_pos.x + kAttackDis - offset.x, 0, _pos.x + kAttackDis - offset.x, 1080, 0x0000ff);
	// 当たり判定を表示
	_pCollider->Draw(offset);
#endif
}