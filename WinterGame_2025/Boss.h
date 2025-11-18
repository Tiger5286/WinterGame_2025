#pragma once
#include "Enemy.h"

class Camera;

enum class BossState
{
	Idle,
	Stun,	// ひるみ
	Tackle,	// 突進
	TackleAndWallRun,	// 突進＆壁走り
	CeilingRun,	// 天井走り
	FallAttack,	// 落下攻撃
};

class Boss : public Enemy
{
public:
	Boss(std::shared_ptr<Player>pPlayer, std::shared_ptr<Camera> pCamera,int handle);
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
	Animation _fallAnim;

	std::shared_ptr<Camera> _pCamera;
};