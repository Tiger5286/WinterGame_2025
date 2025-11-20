#pragma once
#include "Geometry.h"

class Bg
{
public:
	Bg(int handle);
	~Bg();

	void Init();
	void Update();
	void Draw(Vector2 drawOffset);
private:
	int _handle;
};

