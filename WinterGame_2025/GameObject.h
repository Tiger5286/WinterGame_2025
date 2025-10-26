#pragma once
#include "Geometry.h"

class Input;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	void Gravity();

protected:
	int _handle;
	Vector2 _pos;
	Vector2 _vel;
};

