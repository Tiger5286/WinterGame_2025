#pragma once
#include "Geometry.h"
#include <vector>
#include <memory>

class Enemy;

class HPUI
{
public:
	HPUI(int handle,int playerMaxHp);
	~HPUI();

	void Init();
	void Update(int playerHP);
	void Draw(Vector2 drawPlayerPos,const std::vector<std::shared_ptr<Enemy>>& pEnemys);
private:
	int _handle;
	int _playerMaxHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;
};