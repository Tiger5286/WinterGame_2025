#pragma once
#include "GameObject.h"
#include "Geometry.h"

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetAlive(bool isAlive);
	bool GetAlive();

	void SetPos(Vector2 pos);
	void SetIsTurn(bool isTurn);
private:
	bool _isAlive;
	bool _isTurn;
};

