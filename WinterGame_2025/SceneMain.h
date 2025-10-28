#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class Input;
class Player;
class Bullet;
class WalkEnemy;

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Init() override;
	void Update(Input input) override;
	void Draw() override;

private:
	/*‰æ‘œƒnƒ“ƒhƒ‹*/
	int _playerH;
	int _playerShotH;
	int _walkEnemyH;

	int _frameCount;
	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Bullet>> _pBullets;
	std::shared_ptr<WalkEnemy> _pWalkEnemy;
};

