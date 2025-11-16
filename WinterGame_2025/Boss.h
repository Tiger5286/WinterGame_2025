#pragma once
#include "Enemy.h"
class Boss : public Enemy
{
public:
	Boss(std::shared_ptr<Player>pPlayer, int handle);
	~Boss();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;

	Animation _idleAnim;
};