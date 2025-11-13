#pragma once
#include "Enemy.h"
#include "Animation.h"

enum class FlyEnemyState
{
	Idle,
	Move
};

class FlyEnemy : public Enemy
{
public:
	//FlyEnemy(int handle, FlyEnemyState state,std::shared_ptr<Player> pPlayer);
	//FlyEnemy(Vector2 firstPos,int handle, FlyEnemyState state,std::shared_ptr<Player> pPlayer);
	FlyEnemy(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, FlyEnemyState state);
	~FlyEnemy();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;
	float _angle;
	FlyEnemyState _state;
};

