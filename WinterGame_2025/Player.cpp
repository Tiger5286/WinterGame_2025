#include "Player.h"
#include "Dxlib.h"
#include <cassert>
#include "input.h"
#include "BoxCollider.h"
#include "Bullet.h"

namespace
{
	// 描画関連
	constexpr int GRAPH_CUT_W = 40;
	constexpr int GRAPH_CUT_H = 40;
	constexpr float DRAW_SCALE = 4.0f;

	// 当たり判定
	constexpr float COLLIDER_W = 80.0f;
	constexpr float COLLIDER_H = 110.0f;

	constexpr float GROUND_H = 800.0f;

	// 動きの制御関連
	constexpr float JUMP_POWER = -15.0f;
	constexpr int MAX_JUMP_FRAME = 15;

	constexpr float MAX_MOVE_SPEED = 10.0f;
	constexpr float STOP_SPEED = 1.0f;
	constexpr float FRICTION_POWER = 0.3f;
}

Player::Player():
	_jumpFrame(0),
	_isGround(false),
	_isJumping(false),
	_isTurn(false)
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

	_pos += _vel;	// 速度ベクトルを位置に足しこむ

	if (_pos.y > GROUND_H)	// 位置が地面の高さを上回ったら補正
	{
		_pos.y = GROUND_H;
		_isGround = true;
		_vel.y = 0.0f;
	}

	if (_isTurn)
	{
		_shotPos = { _pos.x - COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}
	else
	{
		_shotPos = { _pos.x + COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}

	_collider->SetPos(_pos);
}

void Player::Draw()
{
	DrawRectRotaGraph(_pos.x, _pos.y - GRAPH_CUT_H / 2 * DRAW_SCALE, GRAPH_CUT_W * 0, GRAPH_CUT_H * 2, GRAPH_CUT_W, GRAPH_CUT_H, DRAW_SCALE, 0, _handle, true, _isTurn);
#ifdef _DEBUG
	_collider->Draw();
#endif // _DEBUG
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
	bool isMove = false;
	if (_input.IsPressed("right"))
	{
		_vel.x++;
		isMove = true;
		_isTurn = false;
		if (_vel.x > MAX_MOVE_SPEED)
		{
			_vel.x = MAX_MOVE_SPEED;
		}
	}
	if (_input.IsPressed("left"))
	{
		_vel.x--;
		isMove = true;
		_isTurn = true;
		if (_vel.x < -MAX_MOVE_SPEED)
		{
			_vel.x = -MAX_MOVE_SPEED;
		}
	}
	// 自然に止まる力
	if (!isMove)
	{
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