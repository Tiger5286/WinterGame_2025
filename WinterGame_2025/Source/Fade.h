#pragma once
#include "Geometry.h"

enum class FadeState
{
	NoFade,
	NormalFadeIn,
	NormalFadeOut,
	CircleFadeIn,
	CircleFadeOut
};

class Fade
{
public:
	Fade();
	~Fade();

	void Update();
	void Draw();

	void StartFadeOut();
	void StartFadeIn();

	void StartCircleFadeOut();
	void StartCircleFadeIn();

	bool GetIsFadeEnd();

	void SetCirclePos(const Vector2& pos) { _circlePos = pos; }

	const FadeState GetState() const { return _state; }

private:
	int _fadeCircleH;
	Vector2 _circlePos;
	FadeState _state;
	int _fadeFrame;
};