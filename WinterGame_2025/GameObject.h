#pragma once
#include "Geometry.h"
#include "Animation.h"
#include <memory>

class Input;
class Collider;
class Map;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(Vector2 offset) = 0;

	void MapCollision(Map& map);

	void ChangeAnim(Animation anim);

	void SetPos(Vector2 pos) { _pos = pos; }
	Vector2 GetPos() { return _pos; }

	std::shared_ptr<Collider> GetCollider() { return _collider; }

protected:
	void Gravity();

protected:
	Vector2 _pos;
	Vector2 _vel;
	std::shared_ptr<Collider> _collider;
	Animation _nowAnim;
	bool _isGround;		// ê⁄ínÉtÉâÉO
};