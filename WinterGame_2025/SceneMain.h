#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class Input;
class Player;
class Bullet;
class Enemy;
class WalkEnemy;
class FlyEnemy;

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& manager);
	~SceneMain();

	void Init() override;
	void Update(Input input) override;
	void Draw() override;

private:
	/*‰æ‘œƒnƒ“ƒhƒ‹*/
	int _playerH;
	int _playerShotH;
	int _chargeShotH;
	int _chargeParticleH;
	int _walkEnemyH;
	int _flyEnemyH;

	int _frameCount;
	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Bullet>> _pBullets;
	std::vector<std::shared_ptr<Enemy>> _pEnemys;
};

