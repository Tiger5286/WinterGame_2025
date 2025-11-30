#include "Laser.h"
#include "Dxlib.h"
#include "../Utility/BoxCollider.h"
#include "Player.h"
#include "../Game.h"

namespace
{
	constexpr int kGraphSize = 16;
	constexpr float kDrawScale = 3.0f;

	Vector2 kFrameSize = { kGraphSize,kGraphSize };

	constexpr int kHalfDrawSize = (kGraphSize * kDrawScale) / 2;
	constexpr int kColliderW = 24;

	constexpr int kLauncherAnimIndex = 0;
	constexpr int kLaserAnimIndex = 1;
	constexpr int kImpactAnimIndex = 2;
	constexpr int kAnimNum = 4;
	constexpr int kOneAnimFrame = 6;

	constexpr float kAngle180 = DX_PI_F;
}

Laser::Laser(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, int laserLength,bool isDownward):
	Gimmick(pPlayer),
	_handle(handle),
	_laserLength(laserLength),
	_isDownward(isDownward)
{
	_pos = ChipPosToGamePos(chipPos);
	_pCollider = std::make_shared<BoxCollider>(_pos, Vector2(24, _laserLength * GlobalConstants::kDrawChipSize));
	float laserOffsetY = static_cast<float>(_laserLength) / 2 * GlobalConstants::kDrawChipSize - GlobalConstants::kDrawChipSizeHalf;
	_isDownward ? _pCollider->SetPos(Vector2(_pos.x, _pos.y + laserOffsetY)) : _pCollider->SetPos(Vector2(_pos.x, _pos.y - laserOffsetY));
	

	_launcherAnim.Init(_handle, kLauncherAnimIndex, kFrameSize, kAnimNum, kOneAnimFrame, kDrawScale);
	_laserAnim.Init(_handle, kLaserAnimIndex, kFrameSize, kAnimNum, kOneAnimFrame, kDrawScale);
	_impactAnim.Init(_handle, kImpactAnimIndex, kFrameSize, kAnimNum, kOneAnimFrame, kDrawScale);
	if (!_isDownward)
	{
		_launcherAnim.SetRotate(kAngle180);
		_laserAnim.SetRotate(kAngle180);
		_impactAnim.SetRotate(kAngle180);
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
	printfDx("Laser::Update(Map& map) is called. Please use Laser::Update() instead.\n");
}

void Laser::Update()
{
	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}

	float laserOffsetY = static_cast<float>(_laserLength) / 2 * GlobalConstants::kDrawChipSize - GlobalConstants::kDrawChipSizeHalf;
	_isDownward ? _pCollider->SetPos(Vector2(_pos.x, _pos.y + laserOffsetY)) : _pCollider->SetPos(Vector2(_pos.x, _pos.y - laserOffsetY));

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
		_isDownward ? tempDrawPos.y += kGraphSize * kDrawScale * i : tempDrawPos.y -= kGraphSize * kDrawScale * i;
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
	_pCollider->Draw(offset);
#endif // _DEBUG
}
