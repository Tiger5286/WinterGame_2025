#include "ClearFlag.h"
#include "../Utility/BoxCollider.h"
#include "../Game.h"
#include "Player.h"
#include "DxLib.h"

ClearFlag::ClearFlag(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle) :
	_handle(handle),
	_pPlayer(pPlayer)
{
	_pos = ChipPosToGamePos(chipPos);
	_pCollider = std::make_shared<BoxCollider>(_pos, Vector2(GlobalConstants::kDrawChipSize, GlobalConstants::kDrawChipSize));
}

ClearFlag::~ClearFlag()
{
}

void ClearFlag::Init()
{
}

void ClearFlag::Update(Map& map)
{
}

void ClearFlag::Draw(Vector2 cameraOffset)
{
	DrawRotaGraph(_pos.x - cameraOffset.x, _pos.y - cameraOffset.y, 3.0f, 0.0f, _handle, true);
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
				_pos = ChipPosToGamePos(Vector2(w, h));
				_pCollider->SetPos(_pos);
				return;
			}
		}
	}
}
