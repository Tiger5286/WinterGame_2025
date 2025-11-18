#pragma once
#include "Geometry.h"
class Camera
{
public:
	Camera(Vector2 stageSize);
	~Camera();
	void Init();
	void Update(Vector2 playerPos);

	void Shake(int frame, int power);

	void SetPos(Vector2 pos) { _pos = pos; }
	Vector2 GetPos() const { return _pos; }
	Vector2 GetDrawOffset() const;

private:
	Vector2 _pos;
	Vector2 _stageSize;
	int _shakeFrame;
	bool _shaking;
	int _shakePower;
};

