#pragma once
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class EnemyManager;
class Player;

class BossHPUI
{
public:
	BossHPUI(int handle, Player& player, const std::shared_ptr<EnemyManager> pEnemyManager);
	~BossHPUI();

	void Init();
	void Update();
	void Draw();
private:
	int _handle;
	int _bossMaxHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;

	const Player& _player;
	const std::shared_ptr<EnemyManager> _pEnemyManager;
};