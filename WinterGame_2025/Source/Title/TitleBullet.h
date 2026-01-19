#pragma once
#include "../Systems/Animation.h"
#include <memory>

class Collider;
class TitleEnemy;

class TitleBullet
{
public:
	TitleBullet(TitleEnemy& enemy);
	~TitleBullet();

	void Update();
	void Draw();

	void Shot();
	void OnHit();

	bool GetIsAlive() { return _isAlive; }
public:
	int _handle;

	bool _isAlive = false;
	bool _isHit = false;

	Vector2 _pos;
	Vector2 _vel;

	std::shared_ptr<Collider> _pCollider = nullptr;
	TitleEnemy& _enemy;

	Animation _nowAnim;
	Animation _shotAnim;
	Animation _impactAnim;
};

