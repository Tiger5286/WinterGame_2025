#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>

class Bullet;

class WalkEnemy : public GameObject
{
public:
	WalkEnemy();
	~WalkEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetContext(const std::vector<std::shared_ptr<Bullet>>& pBullets);
private:
	std::vector<std::shared_ptr<Bullet>> _pBullets;
};

