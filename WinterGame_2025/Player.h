#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
};

