#include "Gimmick.h"

namespace
{
	constexpr int kDrawChipSizeHalf = 24;
}

Gimmick::Gimmick(std::shared_ptr<Player> pPlayer) :
	_pPlayer(pPlayer)
{	// マップチップの中心に位置を合わせる
	//_pos = MapChipPosToGamePos(firstPos);
	//_pos = { firstPos.x + DRAW_MAPCHIP_HALF_SIZE,firstPos.y + DRAW_MAPCHIP_HALF_SIZE };
}

Gimmick::~Gimmick()
{
}