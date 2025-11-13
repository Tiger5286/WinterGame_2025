#include "Item.h"
#include "CircleCollider.h"
#include "Animation.h"
#include "Player.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 画像サイズ
	constexpr int GRAPH_SIZE = 16;
	const Vector2 FRAME_SIZE = { GRAPH_SIZE,GRAPH_SIZE };

	// アニメーション関連
	constexpr int COIN_ANIM_NUM = 4;
	constexpr int HEALTH_ANIM_NUM = 7;
	constexpr int ONE_ANIM_FRAME = 6;
	constexpr float DRAW_SCALE = 3.0f;
	constexpr float BIGCOIN_DRAW_SCALE = 5.0f;

	// 当たり判定
	constexpr float COIN_COLLIDER_RADIUS = 16.0f;
	constexpr float BIGCOIN_COLLIDER_RADIUS = 40.0f;
	constexpr float HEALTHITEM_COLLIDER_RADIUS = 24.0f;
}

Item::Item(Vector2 mapChipFirstPos, ItemType type, std::shared_ptr<Player> pPlayer,int handle):
	_type(type),
	_pPlayer(pPlayer),
	_handle(handle),
	_isAlive(true)
{
	if (_type == ItemType::Coin)
	{
		_nowAnim.Init(_handle, 0, FRAME_SIZE, COIN_ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
		_collider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, COIN_COLLIDER_RADIUS);
	}
	else if (_type == ItemType::BigCoin)
	{
		_nowAnim.Init(_handle, 0, FRAME_SIZE, COIN_ANIM_NUM, ONE_ANIM_FRAME, BIGCOIN_DRAW_SCALE);
		_collider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, BIGCOIN_COLLIDER_RADIUS);
	}
	else if (_type == ItemType::HealthItem)
	{
		_nowAnim.Init(_handle, 0, FRAME_SIZE, HEALTH_ANIM_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
		_collider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, HEALTHITEM_COLLIDER_RADIUS);
	}
	else
	{
		assert(false && "不正なItemTypeが指定されました");
	}

	_pos = MapChipPosToGamePos(mapChipFirstPos);
	_collider->SetPosToBox(_pos);
}

Item::~Item()
{
}

void Item::Init()
{
}

void Item::Update(Map& map)
{
	if (_collider->CheckCollision(_pPlayer->GetCollider()))
	{
		GetItem();
	}
	_nowAnim.Update();
}

void Item::Draw(Vector2 offset)
{
	_nowAnim.Draw(_pos - offset, false);
#ifdef _DEBUG
	_collider->Draw(offset);
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