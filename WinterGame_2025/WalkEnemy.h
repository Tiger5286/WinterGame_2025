#pragma once
#include "GameObject.h"
class WalkEnemy : public GameObject
{
public:
	WalkEnemy();
	~WalkEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;
};

