#pragma once
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"

// BulletTypeを使用するためインクルード
#include "../GameObjects/Bullet.h"

class Map;
class Camera;
class Enemy;

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void Update(Map& map, Vector2 cameraPos, std::vector<std::shared_ptr<Enemy>> pEnemys);
	void Draw(Vector2 offset);

	void Shot(BulletType type, Vector2 shotPos, bool isTurn);

private:
	int _shotH = -1;
	int _chargeShotH = -1;

	std::vector<std::shared_ptr<Bullet>> _pBullets;
};

