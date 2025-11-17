#include "Camera.h"
#include "Game.h"

namespace
{
	constexpr int CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;

	constexpr float MOVE_Y_INTERVAL = 250.0f;
}

Camera::Camera(Vector2 stageSize):
	_stageSize(stageSize)
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
	_pos.x = playerPos.x;
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
		_pos.y = playerPos.y - MOVE_Y_INTERVAL;
	}
	else if (playerDisY > MOVE_Y_INTERVAL)
	{
		_pos.y = playerPos.y + MOVE_Y_INTERVAL;
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
}

Vector2 Camera::GetDrawOffset() const
{
	return { _pos.x - GlobalConstants::SCREEN_WIDTH / 2, _pos.y - GlobalConstants::SCREEN_HEIGHT / 2 };
}
