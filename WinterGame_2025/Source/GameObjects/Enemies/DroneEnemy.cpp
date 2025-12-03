#include "DroneEnemy.h"
#include "../../Utility/BoxCollider.h"
#include "../Player.h"
#include "Dxlib.h"

namespace
{
	// 描画関連
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 16;
	const Vector2 kGraphSize = Vector2(kGraphWidth, kGraphHeight);
	constexpr int kAnimNum = 4;
	constexpr int kOneAnimFrame = 6;
	constexpr float kDrawScale = 3.0f;

	// 当たり判定
	constexpr int kColliderWidth = 25 * 3;
	constexpr int kColliderHeight = 13 * 3;

	// 捜索状態時の移動速度
	constexpr float kMoveSpeed = 1.5f;
	constexpr float kAttackLength = 500.0f;	// プレイヤーを見つける距離

	// 攻撃のクールタイム(60から数え始めるので実質120)
	constexpr int kAttackCoolTime = 60 + 120;
	constexpr float kAttackMoveSpeed = 7.0f;	// 攻撃状態の突進の速度
	constexpr int kAttackTime = 60;		// 突進を続ける時間

	constexpr float kReturnAccelY = 0.3f;	// 元の位置に戻るときの加速度
	constexpr float kReturnAccelX = 0.5f;	// 元の位置に戻るときの加速度
	constexpr float kMinReturnSpeedX = 5.0f;	// 元の位置に戻るときのX最低速度
	constexpr float kReturnRange = 20.0f;	// 元の位置だと判定する範囲(この範囲に入ると捜索状態に戻る)

	// hp
	constexpr int kHp = 1;

	// スコア
	constexpr int kScore = 150;
}

DroneEnemy::DroneEnemy(Vector2 firstPos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle):
	Enemy(kHp,kScore,pPlayer,pEffectManager,sceneManager),
	_handle(handle),
	_frame(kAttackCoolTime)
{
	_nowAnim.Init(_handle, 0, kGraphSize, kAnimNum, kOneAnimFrame, kDrawScale);
	_pos = ChipPosToGamePos(firstPos);
	_firstPosY = _pos.y;

	_pCollider = std::make_shared<BoxCollider>(_pos, Vector2(kColliderWidth, kColliderHeight));
}

DroneEnemy::~DroneEnemy()
{
}

void DroneEnemy::Init()
{
}

void DroneEnemy::Update(Map& map)
{
	BaseUpdate();

	auto dist = _pPlayer->GetPos() - _pos;

	if (_frame < kAttackCoolTime)
	{
		_frame++;
	}
	if (_state == DroneEnemyState::Search)
	{
		// プレイヤーの方へ進む
		dist.x < 0 ? _vel.x = -kMoveSpeed : _vel.x = kMoveSpeed;
		_vel.y = 0.0f;
		// プレイヤーが一定範囲内に入る、かつクールタイムが経過していたら攻撃する
		if (dist.Length() < kAttackLength && _frame >= kAttackCoolTime)
		{
			_state = DroneEnemyState::Attack;
			_vel = dist.Normalized() * kAttackMoveSpeed;	// プレイヤーに向かって突進
			_frame = 0;	// カウンターをリセット
		}
	}
	else if (_state == DroneEnemyState::Attack)
	{
		// kAttackTime経過したら既定の位置に戻る
		if (_frame > kAttackTime)
		{
			_state = DroneEnemyState::Return;
		}
	}
	else if (_state == DroneEnemyState::Return)
	{
		_frame = kAttackTime;	// カウンターを固定
		_pos.y - _firstPosY < 0 ? _vel.y += kReturnAccelY : _vel.y -= kReturnAccelY;	// 既定の高さに近づくように加速する
		if (_vel.x < -kMinReturnSpeedX)	// 最低速度を下回らないように減速
		{
			_vel.x += kReturnAccelX;
		}
		if (_vel.x > kMinReturnSpeedX)
		{
			_vel.x -= kReturnAccelX;
		}
		// 既定の高さの範囲に入ったら捜索状態に戻る
		if (abs(_pos.y - _firstPosY) < kReturnRange)
		{
			_state = DroneEnemyState::Search;
			_vel.y = 0.0f;
		}
	}

	_pos += _vel;
	_pCollider->SetPos(_pos);

	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	_nowAnim.Update();
}

void DroneEnemy::Draw(Vector2 offset)
{
	if (_damageFrame > 0)
	{
		SetDrawBright(255, 64, 64);	// ダメージを受けている間は赤くなる
	}
	_nowAnim.Draw(_pos - offset, false);
	SetDrawBright(255, 255, 255);	// 色を元に戻す
#ifdef _DEBUG
	_pCollider->Draw(offset);
	DrawFormatString(_pos.x - offset.x, _pos.y - offset.y - 50, 0xffffff, "%d", _frame);
	DrawCircle(_pos.x - offset.x, _pos.y - offset.y, kAttackLength, 0x0000ff, false);
#endif
}