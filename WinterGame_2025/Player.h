#pragma once
#include "GameObject.h"
#include "Input.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetInput(const Input& input);
private:
	Input _input;
	int _jumpFrame;
	bool _isGround;
	bool _isJumping;
};

