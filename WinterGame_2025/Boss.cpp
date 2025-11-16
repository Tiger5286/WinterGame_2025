#include "Boss.h"
#include "Game.h"
#include "BoxCollider.h"

namespace
{
	const Vector2 GRAPH_SIZE = { 32,36 };
	const Vector2 COLLIDER_SIZE = { 130,150 };
	constexpr float DRAW_SCALE = 6.0f;

	const Vector2 firstChipPos = { 32,18 };
}

Boss::Boss(std::shared_ptr<Player> pPlayer, int handle) :
	Enemy(100, pPlayer),
	_handle(handle)
{
	_pos = ChipPosToGamePos(firstChipPos);
	_pos.y += GlobalConstants::DRAW_CHIP_SIZE_HALF;	// ƒ`ƒbƒv”¼•ª‰º‚É‚¸‚ç‚·
	_collider = std::make_shared<BoxCollider>(_pos, COLLIDER_SIZE);
	_collider->SetPosToBox(_pos);

	_idleAnim.Init(_handle, 0, GRAPH_SIZE, 5, 6, DRAW_SCALE);
	_nowAnim = _idleAnim;
}

Boss::~Boss()
{
}

void Boss::Init()
{
}

void Boss::Update(Map& map)
{
	_nowAnim.Update();
}

void Boss::Draw(Vector2 offset)
{
	_nowAnim.Draw({ _pos.x - offset.x,_pos.y - offset.y - GRAPH_SIZE.y / 2 * DRAW_SCALE }, true);
#ifdef _DEBUG
	_collider->Draw(offset);
#endif
}
