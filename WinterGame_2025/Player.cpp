#include "Player.h"
#include "Dxlib.h"
#include <cassert>
#include "input.h"
#include "BoxCollider.h"
#include "Bullet.h"

namespace
{
	// 描画関連
	constexpr int PLAYER_GRAPH_CUT_W = 40;
	constexpr int PLAYER_GRAPH_CUT_H = 40;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr int CHARGE_PARTICLE_GRAPH_CUT_W = 64;
	constexpr int CHARGE_PARTICLE_GRAPH_CUT_H = 64;

	// 当たり判定
	constexpr float COLLIDER_W = 60.0f;
	constexpr float COLLIDER_H = 80.0f;

	constexpr float GROUND_H = 800.0f;

	// 動きの制御関連
	constexpr float JUMP_POWER = -15.0f;
	constexpr int MAX_JUMP_FRAME = 15;

	constexpr float MOVE_SPEED = 2.0f;
	constexpr float MAX_MOVE_SPEED = 10.0f;
	constexpr float STOP_SPEED = 0.9f;
	constexpr float FRICTION_POWER = 0.7f;

	// ダッシュ関連
	constexpr int DASH_COOL_TIME = 120;
	constexpr float DASH_SPEED = 20.0f;
	constexpr int DASHING_TIME = 15;
}

Player::Player():
	_chargeParticleH(-1),
	_jumpFrame(0),
	_isGround(false),
	_isJumping(false),
	_isTurn(false),
	_dashCoolTime(0),
	_dashFrame(0),
	_isDashing(false),
	_isTurnDashing(false),
	_chargeFrame(0),
	_isCharging(false)
{
	_collider = std::make_shared<BoxCollider>(_pos,Vector2(COLLIDER_W, COLLIDER_H));
}

Player::~Player()
{
}

void Player::Init()
{
	_pos = { 100.0f,100.0f };
}

void Player::Update()
{
	Gravity();	// 重力をかける

	// ジャンプ処理
	Jump();

	// 左右移動処理
	Move();

	// 射撃処理
	Shot();
	ChargeShot();

	// ダッシュ処理
	Dash();

	_pos += _vel;	// 速度ベクトルを位置に足しこむ

	if (_pos.y > GROUND_H)	// 位置が地面の高さを上回ったら補正
	{
		_pos.y = GROUND_H;
		_isGround = true;
		_vel.y = 0.0f;
	}

	if (_isTurn)	// 弾を召喚する位置を設定
	{
		_shotPos = { _pos.x - COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}
	else
	{
		_shotPos = { _pos.x + COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}

	// 当たり判定の位置を設定
	_collider->SetPos(_pos);
}

void Player::Draw()
{
	DrawRectRotaGraph(_pos.x, _pos.y - PLAYER_GRAPH_CUT_H / 2 * DRAW_SCALE, PLAYER_GRAPH_CUT_W * 0, PLAYER_GRAPH_CUT_H * 2, PLAYER_GRAPH_CUT_W, PLAYER_GRAPH_CUT_H, DRAW_SCALE, 0, _handle, true, _isTurn);

	// チャージ中にパーティクルを描画
	if (_isCharging)
	{
		DrawRectRotaGraph(_pos.x, _pos.y - PLAYER_GRAPH_CUT_H / 2 * DRAW_SCALE,
			CHARGE_PARTICLE_GRAPH_CUT_W * 3, 0,
			CHARGE_PARTICLE_GRAPH_CUT_W, CHARGE_PARTICLE_GRAPH_CUT_H, DRAW_SCALE, 0.0f, _chargeParticleH, true);
	}
#ifdef _DEBUG
	_collider->Draw();
#endif // _DEBUG
}

void Player::SetOtherHandle(int chargeParticleH)
{
	_chargeParticleH = chargeParticleH;
}

void Player::SetContext(const Input& input, std::vector<std::shared_ptr<Bullet>>& pBullets)
{
	_input = input;
	_pBullets = pBullets;
}

void Player::Jump()
{
	if (_input.IsPressed("jump") && _isGround)
	{	// ジャンプが入力され、かつ接地しているならジャンプ中
		_isJumping = true;
	}
	if (_input.IsPressed("jump"))
	{	// ジャンプが入力され、かつジャンプ中なら長押し有効
		if (_isJumping)
		{
			_jumpFrame++;
			_isGround = false;
			if (_jumpFrame < MAX_JUMP_FRAME)
			{
				_vel.y = JUMP_POWER;
			}
		}
	}
	else
	{	// ジャンプが入力されていないならジャンプ中を解除
		_jumpFrame = 0;
		_isJumping = false;
	}
}

void Player::Move()
{
	if (_input.IsPressed("right"))
	{
		_vel.x += MOVE_SPEED;
		_isTurn = false;
	}
	if (_input.IsPressed("left"))
	{
		_vel.x -= MOVE_SPEED;
		_isTurn = true;
	}

	// 最高速度以上は出ないようにする
	if (_vel.x > MAX_MOVE_SPEED)
	{
		_vel.x = MAX_MOVE_SPEED;
	}
	if (_vel.x < -MAX_MOVE_SPEED)
	{
		_vel.x = -MAX_MOVE_SPEED;
	}

	// 自然に止まる力
	if (_vel.x >= -STOP_SPEED && _vel.x <= STOP_SPEED)
	{
		_vel.x = 0.0f;
	}
	if (_vel.x > STOP_SPEED)
	{
		_vel.x -= FRICTION_POWER;
	}
	if (_vel.x < -STOP_SPEED)
	{
		_vel.x += FRICTION_POWER;
	}
}

void Player::Shot()
{
	if (_input.IsTriggered("shot"))
	{
		for (auto& bullet : _pBullets)
		{
			if (!bullet->GetAlive())
			{
				bullet->SetAlive(true);
				bullet->SetPos(_shotPos);
				bullet->SetIsTurn(_isTurn);
				break;
			}
		}
	}
}

void Player::ChargeShot()
{
	if (_input.IsPressed("shot"))
	{
		_chargeFrame++;
		if (_chargeFrame > 60)
		{
			_isCharging = true;
		}
	}
	else
	{
		if (_chargeFrame > 60)
		{
			printfDx("チャージ弾発射\n");
		}
		_chargeFrame = 0;
		_isCharging = false;
	}
}

void Player::Dash()
{
	// クールタイムを減らす
	if (_dashCoolTime > 0)
	{
		_dashCoolTime--;
	}
	if (_input.IsTriggered("dash") && _dashCoolTime == 0)
	{	// ダッシュが入力され、かつクールタイムが終わっているなら
		_isTurnDashing = _isTurn;
		_isDashing = true;
		_dashCoolTime = DASH_COOL_TIME;
	}
	if (_isDashing)
	{	// ダッシュ本体の処理
		_dashFrame++;
		_vel.y = 0.0f;
		if (_isTurnDashing)
		{
			_vel.x = -DASH_SPEED;
		}
		else
		{
			_vel.x = DASH_SPEED;
		}
		if (_dashFrame > DASHING_TIME)
		{
			_dashFrame = 0;
			_isDashing = false;
		}
	}
}
