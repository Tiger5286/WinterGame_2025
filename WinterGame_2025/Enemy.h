#pragma once
#include "GameObject.h"
#include <vector>

class Bullet;

class Enemy : public GameObject
{
public:
	Enemy(int hp);
	virtual ~Enemy();

	virtual void Init() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

	void SetHp(int hp) { _hp = hp; }
	int GetHp() const { return _hp; }
	void Delete() { _isAlive = false; }
	bool GetIsAlive()const { return _isAlive; }
	void SetIsHitChargeShot(bool ishit) { _isHitChargeShot = ishit; }
	bool GetIsHitChargeShot()const { return _isHitChargeShot; }
protected:
	int _hp;
	bool _isAlive;
	bool _isHitChargeShot;
};

