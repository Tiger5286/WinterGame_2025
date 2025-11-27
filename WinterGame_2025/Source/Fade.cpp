#include "Fade.h"
#include "Dxlib.h"
#include "Game.h"

namespace
{
	constexpr int kFadeTime = 45;
	constexpr int kCircleGraphSize = 1024;
	constexpr float kCircleScale = 2.5f;
}

Fade::Fade() :
	_state(FadeState::NormalFadeIn),
	_fadeFrame(kFadeTime),
	_circlePos(GlobalConstants::kScreenWidth / 2, GlobalConstants::kScreenHeight / 2)
{
	_fadeCircleH = LoadGraph("data/UI/FadeCircle.png");
}

Fade::~Fade()
{
	DeleteGraph(_fadeCircleH);
}

void Fade::Update()
{
	if (_state == FadeState::NormalFadeIn || _state == FadeState::CircleFadeIn)
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
	else if (_state == FadeState::NormalFadeOut || _state == FadeState::CircleFadeOut)
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
	// フェード進行度を割合で計算
	float fadeProgress = static_cast<float>(_fadeFrame) / static_cast<float>(kFadeTime);
	// 通常フェードの描画
	if (_state == FadeState::NormalFadeOut || _state == FadeState::NormalFadeIn)
	{
		float alpha = fadeProgress * 255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, GlobalConstants::kScreenWidth, GlobalConstants::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else if (_state == FadeState::CircleFadeOut || _state == FadeState::CircleFadeIn)	// 円形フェードの描画
	{
		int CircleDrawTop = _circlePos.y - kCircleGraphSize / 2 * (1 - fadeProgress) * kCircleScale + 1;	// +1と-1は境界線のちらつきを防ぐため
		int CircleDrawBottom = _circlePos.y + kCircleGraphSize / 2 * (1 - fadeProgress) * kCircleScale - 1;
		int CircleDrawLeft = _circlePos.x - kCircleGraphSize / 2 * (1 - fadeProgress) * kCircleScale + 1;
		int CircleDrawRight = _circlePos.x + kCircleGraphSize / 2 * (1 - fadeProgress) * kCircleScale - 1;
		DrawBox(0, 0, CircleDrawRight, CircleDrawTop, 0x000000, true);	// 画面左上から丸の右上
		DrawBox(0,GlobalConstants::kScreenHeight,CircleDrawLeft,CircleDrawTop, 0x000000, true);	// 画面右下から丸の左上
		DrawBox(GlobalConstants::kScreenWidth, 0, CircleDrawRight, CircleDrawBottom, 0x000000, true);	// 画面右上から丸の右下
		DrawBox(GlobalConstants::kScreenWidth, GlobalConstants::kScreenHeight, CircleDrawLeft, CircleDrawBottom, 0x000000, true);	// 画面右下から丸の左下
		DrawRotaGraph(_circlePos.x, _circlePos.y, (1 - fadeProgress) * kCircleScale, 0.0, _fadeCircleH, true);
	}

	//DrawFormatString(100, 100, 0xffffff, "%.2f", fadeProgress);
}

void Fade::StartFadeOut()
{
	_state = FadeState::NormalFadeOut;
	_fadeFrame = 0;
}

void Fade::StartFadeIn()
{
	_state = FadeState::NormalFadeIn;
	_fadeFrame = kFadeTime;
}

void Fade::StartCircleFadeOut()
{
	_state = FadeState::CircleFadeOut;
	_fadeFrame = 0;
}

void Fade::StartCircleFadeIn()
{
	_state = FadeState::CircleFadeIn;
	_fadeFrame = kFadeTime;
}

bool Fade::GetIsFadeEnd()
{
	if ((_state == FadeState::NormalFadeOut || _state == FadeState::CircleFadeOut)&& _fadeFrame == kFadeTime - 1)
	{
		return true;
	}
	return false;
}