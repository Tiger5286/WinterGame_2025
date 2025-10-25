#include "Player.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int PLAYER_CUT_W = 40;
	constexpr int PLAYER_CUT_H = 40;
	constexpr float PLAYER_SCALE = 1.0f;
}

Player::Player()
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
}

void Player::Draw()
{
	DrawRectRotaGraph(_pos.x,_pos.y,PLAYER_CUT_W * 0,PLAYER_CUT_H * 2,PLAYER_CUT_W,PLAYER_CUT_H,PLAYER_SCALE,0,_handle,true);
}