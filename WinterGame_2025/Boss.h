#pragma once
#include "Enemy.h"

enum class BossState
{
	Idle,
	Stun,	// ひるみ
	Tackle	// 突進
};

class Boss : public Enemy
{
public:
	Boss(std::shared_ptr<Player>pPlayer, int handle);
	~Boss();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

private:

	void ChangeState(BossState state);

private:
	int _handle;
	bool _isTurn;

	int _frame;	// いろんな行動に使うフレームカウント

	BossState _state;

	Animation _idleAnim;
	Animation _stunAnim;
	Animation _tackleAnim;
};