#pragma once
#include "Enemy.h"
class JumpEnemy : public Enemy
{
public:
	JumpEnemy(Vector2 firstPos,std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle);
	~JumpEnemy();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;

	int _attackCooltime;
	int _attackFrame;
	bool _isAttacking;
	bool _isTurn;

	Animation _idleAnim;
	Animation _attackAnim;
};

