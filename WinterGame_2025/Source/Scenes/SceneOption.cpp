#include "SceneOption.h"
#include "../Game.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "SceneManager.h"

namespace
{
	// ウィンドウサイズと位置
	constexpr int kWindowLeft = GlobalConstants::kScreenWidth / 4;
	constexpr int kWindowTop = GlobalConstants::kScreenHeight / 4;
	constexpr int kWindowWidth = GlobalConstants::kScreenWidth / 2;
	constexpr int kWindowHeight = GlobalConstants::kScreenHeight / 2;

	constexpr int kSoundVolumeBarWidth = 500;
}

SceneOption::SceneOption(SceneManager& manager):
	SceneBase(manager)
{
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
}

void SceneOption::Draw()
{
	// 半透明黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(kWindowLeft, kWindowTop, kWindowLeft + kWindowWidth, kWindowTop + kWindowHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 音量バー
	DrawLine(GlobalConstants::kScreenWidth / 2 - kSoundVolumeBarWidth / 2, GlobalConstants::kScreenHeight / 2,
		GlobalConstants::kScreenWidth / 2 + kSoundVolumeBarWidth / 2, GlobalConstants::kScreenHeight / 2, 0xffffff, 3);
}
