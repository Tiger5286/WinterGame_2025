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

	void SetContext(const std::vector<std::shared_ptr<Bullet>>& pBullets) { _pBullets = pBullets; }
	int GetHp() const { return _hp; }
protected:
	int _hp;
	std::vector<std::shared_ptr<Bullet>> _pBullets;
};

