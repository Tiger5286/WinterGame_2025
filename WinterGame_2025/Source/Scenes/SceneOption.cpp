#include "SceneOption.h"
#include "../Game.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "SceneManager.h"

namespace
{
	// ウィンドウサイズと位置
	constexpr int kWindowLeft = GlobalConstants::kScreenWidth / 4;
	constexpr int kWindowTop = GlobalConstants::kScreenHeight / 4;
	constexpr int kWindowWidth = GlobalConstants::kScreenWidth / 2;
	constexpr int kWindowHeight = GlobalConstants::kScreenHeight / 2;

	// 音量バーの幅と太さ
	constexpr int kSoundVolumeBarWidth = 500;
	constexpr int kSoundVolumeBarThickness = 3;
	// 現在の音量を表す円
	constexpr int kSoundVolumeCircleRadius = 10;
	constexpr int kSoundVolumeCircleOutlineRadius = 12;
}

SceneOption::SceneOption(SceneManager& manager):
	SceneBase(manager)
{
	_bgmVolume = SoundManager::GetInstance().GetBGMVolume();
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

void SceneOption::Update(Input& input)
{
	if (input.IsTriggered("back"))
	{
		_manager.PopScene();
	}
	if (input.IsPressed("left"))
	{
		_bgmVolume -= 2;
		if (_bgmVolume < 0)
		{
			_bgmVolume = 0;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::BGM, _bgmVolume);
	}
	if (input.IsPressed("right"))
	{
		_bgmVolume += 2;
		if (_bgmVolume > 255)
		{
			_bgmVolume = 255;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::BGM, _bgmVolume);
	}
}

void SceneOption::Draw()
{
	// 半透明黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(kWindowLeft, kWindowTop, kWindowLeft + kWindowWidth, kWindowTop + kWindowHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 音量バー
	DrawLine(GlobalConstants::kScreenWidth / 2 - kSoundVolumeBarWidth / 2, GlobalConstants::kScreenHeight / 2,
		GlobalConstants::kScreenWidth / 2 + kSoundVolumeBarWidth / 2, GlobalConstants::kScreenHeight / 2, 0xffffff, kSoundVolumeBarThickness);
	// 音量バーの現在値
	DrawCircle(GlobalConstants::kScreenWidth / 2 - kSoundVolumeBarWidth / 2 + (_bgmVolume / 255.0f) * kSoundVolumeBarWidth,
		GlobalConstants::kScreenHeight / 2, kSoundVolumeCircleOutlineRadius, 0xffffff, true);
	DrawCircle(GlobalConstants::kScreenWidth / 2 - kSoundVolumeBarWidth / 2 + (_bgmVolume / 255.0f) * kSoundVolumeBarWidth,
		GlobalConstants::kScreenHeight / 2, kSoundVolumeCircleRadius, 0xff0000, true);

}
