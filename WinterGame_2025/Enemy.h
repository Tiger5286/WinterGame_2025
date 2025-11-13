#pragma once
#include "GameObject.h"
#include <vector>

class Player;

class Enemy : public GameObject
{
public:
	Enemy(int hp, std::shared_ptr<Player> pPlayer);
	//Enemy(Vector2 mapChipFirstPos, int hp,std::shared_ptr<Player> pPlayer);
	virtual ~Enemy();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	virtual void Draw(Vector2 offset) override = 0;

	void SetHp(int hp) { _hp = hp; }
	int GetHp() const { return _hp; }
	void Delete() { _isAlive = false; }
	bool GetIsAlive()const { return _isAlive; }
	void SetIsHitChargeShot(bool isHit) { _isHitChargeShot = isHit; }
	bool GetIsHitChargeShot()const { return _isHitChargeShot; }
protected:
	int _hp;
	bool _isAlive;
	bool _isHitChargeShot;
	std::shared_ptr<Player> _pPlayer;
};

