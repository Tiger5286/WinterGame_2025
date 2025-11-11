#pragma once
#include "Geometry.h"
#include "Animation.h"
#include <memory>

class Input;
class Collider;
class Map;

enum class HitDirectionX
{
	None,
	Left,
	Right
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update(Map& map) = 0;
	virtual void Draw(Vector2 offset) = 0;

	bool MapCollision(Map& map);
	bool MapCollision(Map& map, HitDirectionX& hitDirX);
	bool MapCollision(Map& map, bool& isHitUp);

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