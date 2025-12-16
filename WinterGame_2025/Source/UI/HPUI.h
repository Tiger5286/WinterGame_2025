#pragma once
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Enemy;
class Player;

class HPUI
{
public:
	HPUI(int handle,const int playerMaxHp,const Player& player,const std::vector<std::shared_ptr<Enemy>>& pEnemies);
	~HPUI();

	void Init();
	void Update();
	void Draw();
private:
	int _handle;
	const int _playerMaxHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;

	const Player& _player;
	const std::vector<std::shared_ptr<Enemy>>& _pEnemies;
};