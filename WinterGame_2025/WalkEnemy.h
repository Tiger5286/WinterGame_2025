#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>

class WalkEnemy : public Enemy
{
public:
	WalkEnemy();
	~WalkEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;
};

