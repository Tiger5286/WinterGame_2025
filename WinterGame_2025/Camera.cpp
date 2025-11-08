#include "Camera.h"
#include "Game.h"

namespace
{
	constexpr int CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;
}

Camera::Camera(int stageWidth):
	STAGE_WIDTH(stageWidth)
{
	// 初期位置を画面中央に設定(マップチップ0.5個分下にずらす)
	_pos.y = GlobalConstants::SCREEN_HEIGHT / 2 + CHIP_SIZE * DRAW_SCALE / 2;
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Update(float PlayerPosX)
{
	// カメラのX座標をプレイヤーの位置に合わせる
	_pos.x = PlayerPosX;
	// カメラの位置がマップの端を超えないように補正
	if (_pos.x < GlobalConstants::SCREEN_WIDTH / 2)
	{
		_pos.x = GlobalConstants::SCREEN_WIDTH / 2;
	}
	else if (_pos.x > STAGE_WIDTH - GlobalConstants::SCREEN_WIDTH / 2)
	{
		_pos.x = STAGE_WIDTH - GlobalConstants::SCREEN_WIDTH / 2;
	}
}

Vector2 Camera::GetDrawOffset() const
{
	return { _pos.x - GlobalConstants::SCREEN_WIDTH / 2, _pos.y - GlobalConstants::SCREEN_HEIGHT / 2 };
}
