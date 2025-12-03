#pragma once
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"

class Item;
class Player;
class SceneMain;

class ItemManager
{
public:
	ItemManager(std::shared_ptr<Player> pPlayer,SceneMain& sceneMain);
	~ItemManager();

	void Update();
	void Draw(Vector2 offset);

	void LoadItems(const std::vector<uint16_t>& objectData,Size size);

private:
	int _coinH = -1;
	int _bigCoinH = -1;
	int _healthItemH = -1;

	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Item>> _pItems;

	SceneMain& _sceneMain;
};