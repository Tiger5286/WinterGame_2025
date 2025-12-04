#include "SceneTitle.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"
#include "SceneMain.h"
#include "../Game.h"
#include <cassert>
#include <string>
#include <cmath>

namespace
{
	constexpr int kStringFrickerInterval = 60;
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_titleHandle = LoadGraph("data/UI/Title.png");
	assert(_titleHandle != -1);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_titleHandle);
}

void SceneTitle::Init()
{
}

void SceneTitle::Update(Input& input)
{
	_frame++;

	if (input.IsTriggered("decision"))
	{
		if (_manager.GetClearedStage() == Stages::None)
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Tutorial), FadeState::CircleFadeIn, FadeState::NormalFadeOut);
		}
		else
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
		}
	}


#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneTitle::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);

	float sin = sinf(_frame * 0.02f);
	DrawRotaGraph(screenW / 2, screenH / 2 + sin * 15, 1.0, 0.0, _titleHandle, true);

	// "Press A to Start"‚ð“_–Å‚³‚¹‚é
	if (_frame % kStringFrickerInterval < kStringFrickerInterval / 2)
	{
		const std::string pressAString = "Press A to Start";
		int strW = GetDrawStringWidth(pressAString.c_str(), static_cast<int>(pressAString.size()));
		DrawString(screenW / 2 - strW / 2, screenH / 2 + 250, pressAString.c_str(), 0xffffff);
	}
	

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}