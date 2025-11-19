#include "Laser.h"
#include "Dxlib.h"
#include "BoxCollider.h"
#include "Player.h"
#include "Game.h"

namespace
{
	constexpr int GRAPH_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;

	Vector2 FRAME_SIZE = { GRAPH_SIZE,GRAPH_SIZE };

	constexpr int HALF_DRAW_SIZE = (GRAPH_SIZE * DRAW_SCALE) / 2;
	constexpr int COLLIDER_WIDTH = 24;

	constexpr int LAUNCHER_ANIM_INDEX = 0;
	constexpr int LASER_ANIM_INDEX = 1;
	constexpr int IMPACT_ANIM_INDEX = 2;
	constexpr int ANIM_NUM = 4;
	constexpr int ONE_ANIM_FRAME = 6;

	constexpr float ANGLE_180 = DX_PI_F;
}

Laser::Laser(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, int laserLength,bool isDownward):
	Gimmick(pPlayer),
	_handle(handle),
	_laserLength(laserLength),
	_isDownward(isDownward)
{
	_pos = ChipPosToGamePos(chipPos);
	_collider = std::make_shared<BoxCollider>(_pos, Vector2(24, _laserLength * GlobalConstants::DRAW_CHIP_SIZE));
	float laserOffsetY = static_cast<float>(_laserLength) / 2 * GlobalConstants::DRAW_CHIP_SIZE - GlobalConstants::DRAW_CHIP_SIZE_HALF;
	_isDownward ? _collider->SetPos(Vector2(_pos.x, _pos.y + laserOffsetY)) : _collider->SetPos(Vector2(_pos.x, _pos.y - laserOffsetY));
	

	_launcherAnim.Init(_handle, LAUNCHER_ANIM_INDEX, FRAME_SIZE, ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_laserAnim.Init(_handle, LASER_ANIM_INDEX, FRAME_SIZE, ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_impactAnim.Init(_handle, IMPACT_ANIM_INDEX, FRAME_SIZE, ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	if (!_isDownward)
	{
		_launcherAnim.SetRotate(ANGLE_180);
		_laserAnim.SetRotate(ANGLE_180);
		_impactAnim.SetRotate(ANGLE_180);
	}
}

Laser::~Laser()
{
}

void Laser::Init()
{
}

void Laser::Update(Map& map)
{
	// プレイヤーに当たったらダメージを与える
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	// アニメーション更新
	_launcherAnim.Update();
	_laserAnim.Update();
	_impactAnim.Update();
}

void Laser::Draw(Vector2 offset)
{
	Vector2 drawPos = { _pos.x - offset.x, _pos.y - offset.y };
	// レーザーを描画
	for (int i = 0; i < _laserLength; i++)	// レーザーの長さ分ループ
	{
		//drawPos.y = GRAPH_SIZE * DRAW_SCALE * i;
		Vector2 tempDrawPos = drawPos;
		_isDownward ? tempDrawPos.y += GRAPH_SIZE * DRAW_SCALE * i : tempDrawPos.y -= GRAPH_SIZE * DRAW_SCALE * i;
		if (i == 0)	// レーザーの発射口部分
		{
			_launcherAnim.Draw(tempDrawPos, false);
		}
		else if (i == _laserLength - 1)	// レーザーの先端部分
		{
			_impactAnim.Draw(tempDrawPos, false);
		}
		else	// レーザーの中間部分
		{
			_laserAnim.Draw(tempDrawPos, false);
		}
	}
#ifdef _DEBUG
	_collider->Draw(offset);
#endif // _DEBUG
}
