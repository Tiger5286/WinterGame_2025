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

class Map;
class Camera;

class Laser;

// どのステージかを表す列挙体
enum class Stages
{
	Temp,
	Tutorial,
	Stage1,
	Stage2,
	Boss
};

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& manager, Stages stage);
	~SceneMain();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;

	void LoadStage(Stages stage);

private:
	/*画像ハンドル*/
	int _playerH;
	int _playerWhiteH;
	int _playerShotH;
	int _chargeShotH;
	int _chargeParticleH;
	int _walkEnemyH;
	int _flyEnemyH;
	int _jumpEnemyH;
	int _mapChipH;
	int _laserH;

	int _frameCount;	// 経過フレーム数

	/*オブジェクト*/
	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Bullet>> _pBullets;
	std::vector<std::shared_ptr<Enemy>> _pEnemys;
	std::vector<std::shared_ptr<Laser>> _pLasers;
	// その他オブジェクト？
	std::shared_ptr<Map> _pMap;
	std::shared_ptr<Camera> _pCamera;
};

