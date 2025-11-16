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
	WalkEnemy(Vector2 chipPos, std::shared_ptr<Player>pPlayer, int handle, WalkEnemyState state, bool isTurn);
	~WalkEnemy();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

	void SetIsTurn(bool isTurn) { _isTurn = isTurn; }
	void SetState(WalkEnemyState state) { _state = state; }

private:
	void UpdateAnim();

private:
	int _handle;
	bool _isTurn;
	//bool _isHitChargeShot;
	WalkEnemyState _state;
	Animation _idleAnim;
	Animation _moveAnim;
	Animation _fallAnim;
};

