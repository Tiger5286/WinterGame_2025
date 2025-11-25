#include "Camera.h"
#include "Game.h"
#include <cmath>
#include "Dxlib.h"

namespace
{
	constexpr int kChipSize = 16;
	constexpr float kDrawScale = 3.0f;

	constexpr float kMoveYInterval = 150.0f;
}

Camera::Camera() :
	_stageSize(Vector2()),
	_shakeFrame(0),
	_shaking(false),
	_shakePower(0)
{
	// 初期位置を画面中央に設定(マップチップ0.5個分下にずらす)
	_pos.x = GlobalConstants::kScreenWidth / 2;
	_pos.y = GlobalConstants::kScreenHeight / 2 + kChipSize * kDrawScale / 2;
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
	if (_pos.x < GlobalConstants::kScreenWidth / 2)
	{
		_pos.x = GlobalConstants::kScreenWidth / 2;
	}
	else if (_pos.x > _stageSize.x - GlobalConstants::kScreenWidth / 2)
	{
		_pos.x = _stageSize.x - GlobalConstants::kScreenWidth / 2;
	}

	// 上下はプレイヤーよりちょっと遅れてついてくるようにする
	float playerDisY = _pos.y - playerPos.y;
	if (playerDisY < -kMoveYInterval) // プレイヤーよりカメラの方が高い
	{
		//_pos.y = playerPos.y - MOVE_Y_INTERVAL;
		_pos.y = std::lerp(_pos.y, playerPos.y - kMoveYInterval, 0.1f);
	}
	else if (playerDisY > kMoveYInterval)
	{
		//_pos.y = playerPos.y + MOVE_Y_INTERVAL;
		_pos.y = std::lerp(_pos.y, playerPos.y + kMoveYInterval, 0.1f);
	}

	// カメラのY位置がマップの上下の端を超えないように補正
	if (_pos.y < GlobalConstants::kScreenHeight / 2)
	{
		_pos.y = GlobalConstants::kScreenHeight / 2;
	}
	else if (_pos.y > _stageSize.y - GlobalConstants::kScreenHeight / 2)
	{
		_pos.y = _stageSize.y - GlobalConstants::kScreenHeight / 2;
	}

	// 画面揺れ処理
	if (_shaking)
	{
		_shakeFrame--;

		_pos.x = _pos.x + (GetRand(_shakePower * 2) - _shakePower);
		_pos.y = _pos.y + (GetRand(_shakePower * 2) - _shakePower);

		if (_shakeFrame == 0)
		{
			_shaking = false;
		}
	}
}

void Camera::Shake(int frame, int power)
{
	_shaking = true;
	_shakeFrame = frame;
	_shakePower = power;
}

Vector2 Camera::GetDrawOffset() const
{
	return { _pos.x - GlobalConstants::kScreenWidth / 2, _pos.y - GlobalConstants::kScreenHeight / 2 };
}
