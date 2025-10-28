#pragma once
#include "Geometry.h"
#include <memory>

class Input;
class Collider;

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
	std::shared_ptr<Collider> _collider;
};