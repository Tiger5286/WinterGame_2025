#include "Boss.h"
#include "Game.h"
#include "BoxCollider.h"
#include "Player.h"
#include "Dxlib.h"

namespace
{
	const Vector2 GRAPH_SIZE = { 32,36 };
	const Vector2 COLLIDER_SIZE = { 130,150 };
	constexpr float DRAW_SCALE = 6.0f;

	const Vector2 firstChipPos = { 32,18 };
}

Boss::Boss(std::shared_ptr<Player> pPlayer, int handle) :
	Enemy(100, pPlayer),
	_handle(handle),
	_isTurn(true),
	_frame(0),
	_state(BossState::Idle)
{
	_pos = ChipPosToGamePos(firstChipPos);
	_pos.y += GlobalConstants::DRAW_CHIP_SIZE_HALF;	// チップ半分下にずらす
	_collider = std::make_shared<BoxCollider>(_pos, COLLIDER_SIZE);
	_collider->SetPosToBox(_pos);

	_idleAnim.Init(_handle, 0, GRAPH_SIZE, 5, 6, DRAW_SCALE);
	_stunAnim.Init(_handle, 1, GRAPH_SIZE, 8, 10, DRAW_SCALE);
	//_stunAnim.SetRotate(DX_PI_F);
	_tackleAnim.Init(_handle, 1, GRAPH_SIZE, 8, 3, DRAW_SCALE);
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
#endif // _DEBUG

	// 待機状態の時の処理
	if (_state == BossState::Idle)
	{
		ChangeAnim(_idleAnim);
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
	// スタン時の処理
	if (_state == BossState::Stun)
	{
		ChangeAnim(_stunAnim);
		_vel = Vector2::Vlerp(_vel, Vector2(), 0.1f);
	}
	//タックル攻撃の時の処理
	if (_state == BossState::Tackle)
	{
		_frame++;
		// 最初は走るモーションだけ
		if (_frame == 1)
		{
			ChangeAnim(_tackleAnim);
		}

		// 走り出す
		if (_frame == 60)
		{
			_isTurn ? _vel.x = -10.0f : _vel.x = 10.0f;
		}
		// どっちかの壁にぶつかったら
		if (_hitDir.right || _hitDir.left)
		{
			// 跳ね返ってスタン
			_vel.y = -10.0f;
			_isTurn ? _vel.x = 5.0f : _vel.x = -5.0f;
			ChangeState(BossState::Stun);
			_frame = 0;
		}
	}

	Gravity();

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
