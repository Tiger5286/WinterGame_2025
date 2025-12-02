#pragma once
#include "../GameObject.h"
class Effect : public GameObject
{
public:
	Effect(Vector2 pos);
	virtual ~Effect();

	void Init() override;
	void Update(Map& map) override;
	void Update();
	void Draw(Vector2 offset) override;

	bool GetIsAlive() { return _isAlive; }

protected:
	bool _isAlive = true;
};

