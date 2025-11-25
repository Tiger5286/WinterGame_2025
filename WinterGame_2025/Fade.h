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
	void StartFadeIn();

	bool GetIsFadeEnd();

	const FadeState GetState() const { return _state; }

private:
	FadeState _state;
	int _fadeFrame;
};