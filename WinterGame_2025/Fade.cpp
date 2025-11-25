#include "Fade.h"
#include "Dxlib.h"
#include "Game.h"

namespace
{
	constexpr int kFadeTime = 60;
}

Fade::Fade():
	_state(FadeState::FadeIn),
	_fadeFrame(kFadeTime)
{
}

Fade::~Fade()
{
}

void Fade::Update()
{
	if (_state == FadeState::FadeIn)
	{
		if (_fadeFrame > 0)
		{
			_fadeFrame--;
		}
		else if (_fadeFrame == 0)
		{
			_state = FadeState::NoFade;
		}
	}
	else if (_state == FadeState::FadeOut)
	{
		if (_fadeFrame < kFadeTime)
		{
			_fadeFrame++;
		}
		else if (_fadeFrame == kFadeTime)
		{
			_state = FadeState::NoFade;
		}
	}
}

void Fade::Draw()
{
	float alpha = static_cast<float>(_fadeFrame) / static_cast<float>(kFadeTime);
	alpha *= 255;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, GlobalConstants::kScreenWidth, GlobalConstants::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawFormatString(100, 100, 0xffffff, "%.2f", alpha);
}

void Fade::StartFadeOut()
{
	_state = FadeState::FadeOut;
	_fadeFrame = 0;
}

void Fade::StartFadeIn()
{
	_state = FadeState::FadeIn;
	_fadeFrame = kFadeTime;
}
