#include "Item.h"
#include "../Utility/CircleCollider.h"
#include "../Systems/Animation.h"
#include "Player.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 画像サイズ
	constexpr int kGraphSize = 16;
	const Vector2 kFrameSize = { kGraphSize,kGraphSize };

	// アニメーション関連
	constexpr int kCoinAnimNum = 4;
	constexpr int kHealthAnimNum = 7;
	constexpr int kOneAnimFrame = 6;
	constexpr float kDrawScale = 3.0f;
	constexpr float kBigCoinDrawScale = 5.0f;

	// 当たり判定
	constexpr float kCoinColliderR = 16.0f;
	constexpr float kBigCoinColliderR = 40.0f;
	constexpr float kHealthItemColliderR = 24.0f;
}

Item::Item(Vector2 mapChipFirstPos, ItemType type, std::shared_ptr<Player> pPlayer,int handle):
	_type(type),
	_pPlayer(pPlayer),
	_handle(handle),
	_isAlive(true)
{
	if (_type == ItemType::Coin)
	{
		_nowAnim.Init(_handle, 0, kFrameSize, kCoinAnimNum, kOneAnimFrame, kDrawScale);
		_pCollider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, kCoinColliderR);
	}
	else if (_type == ItemType::BigCoin)
	{
		_nowAnim.Init(_handle, 0, kFrameSize, kCoinAnimNum, kOneAnimFrame, kBigCoinDrawScale);
		_pCollider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, kBigCoinColliderR);
	}
	else if (_type == ItemType::HealthItem)
	{
		_nowAnim.Init(_handle, 0, kFrameSize, kHealthAnimNum, kOneAnimFrame, kDrawScale);
		_pCollider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, kHealthItemColliderR);
	}
	else
	{
		assert(false && "不正なItemTypeが指定されました");
	}

	_pos = ChipPosToGamePos(mapChipFirstPos);
	_pCollider->SetPosToBox(_pos);
}

Item::~Item()
{
}

void Item::Init()
{
}

void Item::Update(Map& map)
{
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		GetItem();
	}
	_nowAnim.Update();
}

void Item::Draw(Vector2 offset)
{
	_nowAnim.Draw(_pos - offset, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif // _DEBUG
}

void Item::GetItem()
{
	switch (_type)
	{
	case ItemType::Coin:
		printfDx("コインを取得しました\n");
		break;
	case ItemType::BigCoin:
		printfDx("大コインを取得しました\n");
		break;
	case ItemType::HealthItem:
		printfDx("体力回復アイテムを取得しました\n");
		break;
	default:
		assert(false && "不正なItemTypeが指定されました");
		break;
	}
	// アイテム消去
	_isAlive = false;
}