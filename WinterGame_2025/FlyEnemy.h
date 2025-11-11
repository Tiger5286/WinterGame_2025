#pragma once
#include "Enemy.h"
#include "Animation.h"
class FlyEnemy : public Enemy
{
public:
	FlyEnemy(int handle, std::shared_ptr<Player> pPlayer);
	FlyEnemy(Vector2 firstPos,int handle, std::shared_ptr<Player> pPlayer);
	~FlyEnemy();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;

	float _angle;
};

