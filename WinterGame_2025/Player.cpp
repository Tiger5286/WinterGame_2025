#include "Player.h"
#include "Dxlib.h"
#include <cassert>
#include "input.h"

namespace
{
	constexpr int PLAYER_CUT_W = 40;
	constexpr int PLAYER_CUT_H = 40;
	constexpr float PLAYER_SCALE = 4.0f;

	constexpr float GROUND_HEIGHT = 800.0f;

	constexpr float JUMP_POWER = -15.0f;
	constexpr int MAX_JUMP_FRAME = 15;

	constexpr float MAX_MOVE_SPEED = 10.0f;
	constexpr float STOP_SPEED = 1.0f;
	constexpr float FRICTION_POWER = 0.3f;
}

Player::Player():
	_jumpFrame(0)
{
	_handle = LoadGraph("data/PlayerSprites.png");
	assert(_handle != -1);
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
	if (_input.IsPressed("jump"))
	{
		_jumpFrame++;
		if (_jumpFrame < MAX_JUMP_FRAME)
		{
			_vel.y = JUMP_POWER;
		}
	}
	else
	{
		_jumpFrame = 0;
	}

	// 左右移動処理
	bool isMove = false;
	if (_input.IsPressed("right"))
	{
		_vel.x++;
		isMove = true;
		if (_vel.x > MAX_MOVE_SPEED)
		{
			_vel.x = MAX_MOVE_SPEED;
		}
	}
	if (_input.IsPressed("left"))
	{
		_vel.x--;
		isMove = true;
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

	_pos += _vel;	// 速度ベクトルを位置に足しこむ

	if (_pos.y > GROUND_HEIGHT)	// 位置が地面の高さを上回ったら補正
	{
		_pos.y = GROUND_HEIGHT;
		_vel.y = 0.0f;
	}
}

void Player::Draw()
{
	DrawRectRotaGraph(_pos.x,_pos.y,PLAYER_CUT_W * 0,PLAYER_CUT_H * 2,PLAYER_CUT_W,PLAYER_CUT_H,PLAYER_SCALE,0,_handle,true);
}

void Player::SetInput(const Input& input)
{
	_input = input;
}
