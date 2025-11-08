#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>
#include "Animation.h"

enum class WalkEnemyState
{
	Idle,
	Move
};

class WalkEnemy : public Enemy
{
public:
	//WalkEnemy();
	WalkEnemy(int handle, std::shared_ptr<Player>pPlayer);
	~WalkEnemy();

	void Init() override;
	void Update() override;
	void Draw(Vector2 offset) override;

	void SetIsTurn(bool isTurn) { _isTurn = isTurn; }
	void SetState(WalkEnemyState state) { _state = state; }

private:
	int _handle;
	bool _isTurn;
	bool _isHitChargeShot;
	WalkEnemyState _state;
	Animation _idleAnim;
	Animation _moveAnim;
	Animation _fallAnim;
};

