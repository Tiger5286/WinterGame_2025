#pragma once
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"

// BulletTypeを使用するためインクルード
#include "../GameObjects/Bullet.h"

class Map;
class Camera;
class Enemy;
class Player;

class FallBall;

class BulletManager
{
public:
	BulletManager(std::shared_ptr<Player> pPlayer);
	~BulletManager();

	void Update(Map& map, Vector2 cameraPos, std::vector<std::shared_ptr<Enemy>> pEnemys);
	void Draw(Vector2 offset);

	void Shot(BulletType type, Vector2 shotPos, bool isTurn);
	void ShotFallBall(Vector2 pos);

private:
	int _shotH = -1;
	int _chargeShotH = -1;
	int _fallBallH = -1;

	std::vector<std::shared_ptr<Bullet>> _pBullets;
	std::vector<std::shared_ptr<FallBall>> _pFallBalls;

	std::shared_ptr<Player> _pPlayer;
};