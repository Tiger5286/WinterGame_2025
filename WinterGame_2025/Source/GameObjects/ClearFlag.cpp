#include "ClearFlag.h"
#include "../Utility/BoxCollider.h"
#include "../Game.h"
#include "Player.h"
#include "DxLib.h"
#include <cassert>

namespace
{
	constexpr float kDrawScale = 8.0f;

	const Vector2 kGraphSize = { 32,32 };
	constexpr int kIdleAnimNum = 6;
	constexpr int kOneAnimFrame = 6;

	const Vector2 kColliderSize = { 8,64 };
}

ClearFlag::ClearFlag(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle) :
	_handle(handle),
	_pPlayer(pPlayer)
{
	_pos = ChipPosToGamePos(chipPos);
	_pCollider = std::make_shared<BoxCollider>(_pos, kColliderSize);

	_anim.Init(_handle, 0, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
}

ClearFlag::~ClearFlag()
{
	// ‰½‚à‚µ‚È‚¢
}

void ClearFlag::Init()
{
	// ‰½‚à‚µ‚È‚¢
}

void ClearFlag::Update(Map& map)
{
	assert(false && "ClearFlag::Update(Map& map)‚ªŒÄ‚Î‚ê‚Ä‚¢‚Ü‚·BClearFlag::Update()‚ðŽg—p‚µ‚Ä‚­‚¾‚³‚¢");
}

void ClearFlag::Update()
{
	_anim.Update();
}

void ClearFlag::Draw(Vector2 cameraOffset)
{
	_anim.Draw(_pos - cameraOffset, false);
#ifdef _DEBUG
	_pCollider->Draw(cameraOffset);
#endif // _DEBUG
}

void ClearFlag::InitPosFromStage(std::vector<uint16_t>& objectData, Size mapSize)
{
	for (int w = 0; w < mapSize.w; w++)
	{
		for (int h = 0; h < mapSize.h; h++)
		{
			int index = h * mapSize.w + w;
			auto data = static_cast<ObjectData>(objectData[index]);
			if (data == ObjectData::ClearFlag)
			{
				_pos = ChipPosToGamePos(Vector2(static_cast<float>(w), static_cast<float>(h)));
				_pos.y -= kGraphSize.y * kDrawScale / 2;
				_pCollider->SetPos(_pos);
				return;
			}
		}
	}
}
