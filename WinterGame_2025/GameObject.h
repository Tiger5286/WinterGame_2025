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

	void SetPos(Vector2 pos) { _pos = pos; }
	Vector2 GetPos() { return _pos; }

	std::shared_ptr<Collider> GetCollider();

protected:
	void Gravity();

protected:
	Vector2 _pos;
	Vector2 _vel;
	std::shared_ptr<Collider> _collider;
};