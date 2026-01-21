#include "Item.h"
#include "../Utility/CircleCollider.h"
#include "../Utility/BoxCollider.h"
#include "../Systems/Animation.h"
#include "../Systems/SoundManager.h"
#include "Player.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 画像サイズ
	constexpr int kGraphSize = 16;
	const Vector2 kFrameSize = { kGraphSize,kGraphSize };
	const Vector2 kGoldenStatueFrameSize = { 40,40 };

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
	const Vector2 kGoldenStatueColliderSize = { 100,100 };

	// スコア
	constexpr int kCoinScore = 100;
	constexpr int kBigCoinScore = 1000;
	constexpr int kHealthItemScore = 300;
	constexpr int kGoldenStatueScore = 10000;

	constexpr int kPlayerHealAmount = 1;
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
		_score = kCoinScore;
	}
	else if (_type == ItemType::BigCoin)
	{
		_nowAnim.Init(_handle, 0, kFrameSize, kCoinAnimNum, kOneAnimFrame, kBigCoinDrawScale);
		_pCollider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, kBigCoinColliderR);
		_score = kBigCoinScore;
	}
	else if (_type == ItemType::HealthItem)
	{
		_nowAnim.Init(_handle, 0, kFrameSize, kHealthAnimNum, kOneAnimFrame, kDrawScale);
		_pCollider = std::make_shared<CircleCollider>(Vector2{ 0.0f,0.0f }, kHealthItemColliderR);
		_score = kHealthItemScore;
	}
	else if (_type == ItemType::GoldenStatue)
	{
		_nowAnim.Init(_handle, 0, kGoldenStatueFrameSize, 1, kOneAnimFrame, kDrawScale);
		_pCollider = std::make_shared<BoxCollider>(Vector2{ 0.0f,0.0f }, kGoldenStatueColliderSize);
		_score = kGoldenStatueScore;
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
	// 何もしない
}

void Item::Init()
{
	// 何もしない
}

void Item::Update(Map& map)
{
	printfDx("Item::Update(Map& map)が呼ばれました。Item::Update()を使用してください。\n");
}

void Item::Update()
{
	// プレイヤーに当たったら取得処理
	if (_pCollider->CheckCollision(_pPlayer->GetCollider()))
	{
		GetItem();
	}
	// アニメーション更新
	_nowAnim.Update();
}

void Item::Draw(Vector2 offset)
{
	auto drawPos = _pos - offset;
	if (_type == ItemType::GoldenStatue)
	{
		drawPos.y -= 35;
	}
	_nowAnim.Draw(drawPos, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif // _DEBUG
}

void Item::GetItem()
{
	switch (_type)
	{
	case ItemType::Coin:
		SoundManager::GetInstance().PlaySoundGame("Coin");
		break;
	case ItemType::BigCoin:
		SoundManager::GetInstance().PlaySoundGame("BigCoin");
		break;
	case ItemType::HealthItem:
		_pPlayer->Heal(kPlayerHealAmount);
		SoundManager::GetInstance().PlaySoundGame("Heal");
		break;
	case ItemType::GoldenStatue:
		SoundManager::GetInstance().PlaySoundGame("BigCoin");
		break;
	default:
		assert(false && "不正なItemTypeが指定されました");
		break;
	}
	// アイテム消去
	_isAlive = false;
}