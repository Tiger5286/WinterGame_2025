#include "ClearFlag.h"
#include "BoxCollider.h"
#include "Game.h"
#include "Player.h"
#include "SceneMain.h"
#include "DxLib.h"

namespace
{

}

ClearFlag::ClearFlag(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle) :
	_handle(handle),
	_pPlayer(pPlayer)
{
	_pos = ChipPosToGamePos(chipPos);
	_collider = std::make_shared<BoxCollider>(_pos, Vector2(GlobalConstants::DRAW_CHIP_SIZE, GlobalConstants::DRAW_CHIP_SIZE));
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
	_collider->Draw(cameraOffset);
#endif // _DEBUG
}
