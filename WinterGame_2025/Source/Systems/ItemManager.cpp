#include "ItemManager.h"
#include "../Game.h"
#include "Dxlib.h"
#include <cassert>

#include "../GameObjects/Item.h"

ItemManager::ItemManager(std::shared_ptr<Player> pPlayer):
	_pPlayer(pPlayer)
{
	_coinH = LoadGraph("data/Items/Coin.png");
	assert(_coinH != -1);
	_bigCoinH = LoadGraph("data/items/BigCoin.png");
	assert(_bigCoinH != -1);
	_healthItemH = LoadGraph("data/Items/HealthItem.png");
	assert(_healthItemH != -1);
}

ItemManager::~ItemManager()
{
	DeleteGraph(_coinH);
	DeleteGraph(_bigCoinH);
	DeleteGraph(_healthItemH);
}

void ItemManager::Update()
{
	// アイテム制御
	for (auto& item : _pItems)
	{
		if (item != nullptr)
		{
			item->Update();
			if (!item->GetIsAlive())
			{	// 取られたアイテムをvectorから削除する(あんま意味わからん)
				_pItems.erase(
					std::remove_if(_pItems.begin(), _pItems.end(),
						[](const std::shared_ptr<Item>& item) {
							return !item->GetIsAlive();
						}),
					_pItems.end()
				);
			}
		}
	}
}

void ItemManager::Draw(Vector2 offset)
{
	for (auto& item : _pItems)
	{
		item->Draw(offset);
	}
}

void ItemManager::LoadItems(const std::vector<uint16_t>& objectData, Size size)
{
	for (int w = 0; w < size.w; w++)
	{
		for (int h = 0; h < size.h; h++)
		{
			auto index = w + h * size.w;
			auto data = static_cast<ObjectData>(objectData[index]);
			Vector2 pos = Vector2(static_cast<float>(w), static_cast<float>(h));
			switch (data)
			{
			case ObjectData::Coin:
				_pItems.push_back(std::make_shared<Item>(pos, ItemType::Coin, _pPlayer, _coinH));
				break;
			case ObjectData::BigCoin:
				_pItems.push_back(std::make_shared<Item>(pos, ItemType::BigCoin, _pPlayer, _bigCoinH));
				break;
			case ObjectData::HealthItem:
				_pItems.push_back(std::make_shared<Item>(pos, ItemType::HealthItem, _pPlayer, _healthItemH));
				break;
			default:
				break;
			}
		}
	}
}