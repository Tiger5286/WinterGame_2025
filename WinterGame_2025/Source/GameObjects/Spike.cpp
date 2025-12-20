#include "Spike.h"
#include "Dxlib.h"
#include "../Utility/CircleCollider.h"
#include "Player.h"

namespace
{
	constexpr float kDrawScale = 3.0f;

	constexpr float kColliderRadius = 16.0f;

	constexpr int kTurnInterval = 30;
}

Spike::Spike(const Vector2& chipPos, std::shared_ptr<Player> pPlayer, int handle):
	Gimmick(pPlayer),
	_handle(handle)
{
	_pos = ChipPosToGamePos(chipPos);
	_pCollider = std::make_shared<CircleCollider>(_pos, kColliderRadius);
	_pCollider->SetPos(_pos);
}

Spike::~Spike()
{
}

void Spike::Init()
{
}

void Spike::Update(Map& map)
{
}

void Spike::Update()
{
	// プレイヤーに当たったらダメージを与える
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		_pPlayer->TakeDamage();
	}
}

void Spike::Draw(Vector2 offset)
{
	_frame++;
	bool isTurn = _frame % (kTurnInterval * 2) < kTurnInterval;
	DrawRotaGraph(_pos.x - offset.x, _pos.y - offset.y,
	kDrawScale, 0.0, _handle, true, isTurn);
	
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}
