#pragma once
#include "Geometry.h"

class HPUI
{
public:
	HPUI(int handle,int playerMaxHp);
	~HPUI();

	void Init();
	void Update(int playerHP);
	void Draw(Vector2 drawPlayerPos);
private:
	int _handle;
	int _playerMaxHp;
	int _barLength;
	int _drawBarLength;
	int _alpha;
};

