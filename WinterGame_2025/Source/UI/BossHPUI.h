#pragma once
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Enemy;

class BossHPUI
{
public:
	BossHPUI(int handle, int bossMaxHp);
	~BossHPUI();

	void Init();
	void Update(int bossHp);
	void Draw(Vector2 drawPlayerPos, const std::vector<std::shared_ptr<Enemy>>& pEnemys);
private:
	int _handle;
	int _bossMaxHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;
};