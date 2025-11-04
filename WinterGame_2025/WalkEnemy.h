#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>
#include "Animation.h"

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
	bool _isTurn;
	bool _isHitChargeShot;
	Animation _idleAnim;
	Animation _moveAnim;
	Animation _fallAnim;
};

