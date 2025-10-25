#pragma once
#include "Geometry.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

protected:
	int _handle;
	Vector2 _pos;
};

