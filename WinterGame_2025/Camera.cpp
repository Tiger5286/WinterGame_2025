#include "Camera.h"
#include "Game.h"
#include <cmath>
#include "Dxlib.h"

namespace
{
	constexpr int CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr float MOVE_Y_INTERVAL = 150.0f;
}

Camera::Camera(Vector2 stageSize):
	_stageSize(stageSize),
	_shakeFrame(0),
	_shaking(false),
	_shakePower(0)
{
	// 初期位置を画面中央に設定(マップチップ0.5個分下にずらす)
	_pos.x = GlobalConstants::SCREEN_WIDTH / 2;
	_pos.y = GlobalConstants::SCREEN_HEIGHT / 2 + CHIP_SIZE * DRAW_SCALE / 2;
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Update(Vector2 playerPos)
{
	// カメラのX座標をプレイヤーの位置に合わせる
	//_pos.x = playerPos.x;
	_pos.x = std::lerp(_pos.x, playerPos.x, 0.1f);
	// カメラの位置がマップの端を超えないように補正
	if (_pos.x < GlobalConstants::SCREEN_WIDTH / 2)
	{
		_pos.x = GlobalConstants::SCREEN_WIDTH / 2;
	}
	else if (_pos.x > _stageSize.x - GlobalConstants::SCREEN_WIDTH / 2)
	{
		_pos.x = _stageSize.x - GlobalConstants::SCREEN_WIDTH / 2;
	}

	// 上下はプレイヤーよりちょっと遅れてついてくるようにする
	float playerDisY = _pos.y - playerPos.y;
	if (playerDisY < -MOVE_Y_INTERVAL) // プレイヤーよりカメラの方が高い
	{
		//_pos.y = playerPos.y - MOVE_Y_INTERVAL;
		_pos.y = std::lerp(_pos.y, playerPos.y - MOVE_Y_INTERVAL, 0.1f);
	}
	else if (playerDisY > MOVE_Y_INTERVAL)
	{
		//_pos.y = playerPos.y + MOVE_Y_INTERVAL;
		_pos.y = std::lerp(_pos.y, playerPos.y + MOVE_Y_INTERVAL, 0.1f);
	}

	// カメラのY位置がマップの上下の端を超えないように補正
	if (_pos.y < GlobalConstants::SCREEN_HEIGHT / 2)
	{
		_pos.y = GlobalConstants::SCREEN_HEIGHT / 2;
	}
	else if (_pos.y > _stageSize.y - GlobalConstants::SCREEN_HEIGHT / 2)
	{
		_pos.y = _stageSize.y - GlobalConstants::SCREEN_HEIGHT / 2;
	}

	if (_shaking)
	{
		_shakeFrame--;

		_pos.x = _pos.x + GetRand(_shakePower);
		_pos.y = _pos.y + GetRand(_shakePower);

		if (_shakeFrame == 0)
		{
			_shaking = false;
			SRand(0);
		}
	}
}

void Camera::Shake(int frame, int power)
{
	SRand(-power);
	_shaking = true;
	_shakeFrame = frame;
	_shakePower = power;
}

Vector2 Camera::GetDrawOffset() const
{
	return { _pos.x - GlobalConstants::SCREEN_WIDTH / 2, _pos.y - GlobalConstants::SCREEN_HEIGHT / 2 };
}
