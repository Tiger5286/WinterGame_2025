#pragma once
#include "Enemy.h"
#include "Animation.h"
class FlyEnemy : public Enemy
{
public:
	FlyEnemy(int handle,std::shared_ptr<Player> pPlayer);
	~FlyEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;
private:
	int _handle;

	float _angle;
};

