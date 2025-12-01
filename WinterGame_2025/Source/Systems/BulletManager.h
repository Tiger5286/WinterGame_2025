#pragma once
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"

class Bullet;

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
private:
	int _shotH = -1;
	int _chargeShotH = -1;

	std::vector<std::shared_ptr<Bullet>> _pBullets;
};

