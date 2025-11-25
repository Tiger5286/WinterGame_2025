#pragma once

enum class FadeState
{
	NoFade,
	FadeIn,
	FadeOut
};

class Fade
{
public:
	Fade();
	~Fade();

	void Update();
	void Draw();

	void StartFadeOut();

private:
	FadeState _state;
	int _fadeFrame;
};