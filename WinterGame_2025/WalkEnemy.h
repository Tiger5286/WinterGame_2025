#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>

class WalkEnemy : public Enemy
{
public:
	//WalkEnemy();
	WalkEnemy(int handle);
	~WalkEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	int _handle;
	bool _isHitChargeShot;
};

