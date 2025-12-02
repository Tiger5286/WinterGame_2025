#pragma once
#include "Enemy.h"

class Camera;
class Gimmick;

enum class WalkBossState
{
	Idle,
	Stun,	// ひるみ
	Tackle,	// 突進
	TackleAndWallRun,	// 突進＆壁走り
	CeilingRun,	// 天井走り
	FallAttack,	// 落下攻撃
};

class WalkBoss : public Enemy
{
public:
	WalkBoss(std::shared_ptr<Player>pPlayer,std::shared_ptr<EffectManager> pEffectManager, std::shared_ptr<Camera> pCamera, std::shared_ptr<Gimmick> pLaser, int handle);
	~WalkBoss();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

private:

	void ChangeState(WalkBossState state);

private:
	int _handle;
	bool _isTurn;

	int _frame;	// いろんな行動に使うフレームカウント

	WalkBossState _state;

	Animation _idleAnim;
	Animation _stunAnim;
	Animation _tackleAnim;
	Animation _fallAnim;

	std::shared_ptr<Camera> _pCamera;
	std::shared_ptr<Gimmick> _pLaser;
};