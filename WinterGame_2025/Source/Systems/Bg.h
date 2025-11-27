#pragma once
#include "../Utility/Geometry.h"

class Bg
{
public:
	Bg(int bgH,int subBgH);
	~Bg();

	void Init();
	void Update();
	void Draw(Vector2 drawOffset);

private:

	void DrawBg(Vector2 drawOffset);
	void DrawSubBg(Vector2 drawOffset);

private:
	int _bgH;
	int _subBgH;
};

