#pragma once
#include "../Systems/Animation.h"
#include <memory>

class Collider;
class EffectManager;

class TitleEnemy
{
public:
	TitleEnemy(EffectManager& effectManager);
	~TitleEnemy();

	void Update();
	void Draw();

	void OnHit();

	std::shared_ptr<Collider> GetCollider() const { return _pCollider; }
private:
	int _handle;

	EffectManager& _effectManager;

	int _hp = 3;
	int _damageFrame = 0;

	Vector2 _vel;
	Vector2 _pos;

	std::shared_ptr<Collider> _pCollider = nullptr;

	Animation _nowAnim;
	Animation _idleAnim;
	Animation _fallAnim;
};

