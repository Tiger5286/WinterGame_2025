#pragma once
#include "GameObject.h"

class  Player;

enum ItemType
{
	Coin,
	BigCoin,
	HealthItem	// 体力回復アイテム
};

class Item :
    public GameObject
{
public:
	Item(Vector2 mapChipFirstPos,ItemType type,std::shared_ptr<Player> pPlayer,int handle);
	~Item();
	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

	bool GetIsAlive() const { return _isAlive; }

private:
	void GetItem();

private:
	/*画像ハンドル*/
	int _handle;

	bool _isAlive;

	ItemType _type;
	std::shared_ptr<Player> _pPlayer;
};