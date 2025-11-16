#pragma once
#include "Geometry.h"
class Camera
{
public:
	Camera(int stageWidth);
	~Camera();
	void Init();
	void Update(float PlayerPosX);

	Vector2 GetPos() const { return _pos; }
	Vector2 GetDrawOffset() const;

private:
	Vector2 _pos;
	const int STAGE_WIDTH;
};

