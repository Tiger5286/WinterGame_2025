#pragma once
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Enemy;

class HPUI
{
public:
	HPUI(int handle,const int playerMaxHp,const int& playerHp);
	~HPUI();

	void Init();
	void Update();
	void Draw(Vector2 drawPlayerPos,const std::vector<std::shared_ptr<Enemy>>& pEnemys);
private:
	int _handle;
	const int _playerMaxHp;
	const int& _playerHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;
};