#include "SceneGameOver.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "../Game.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"

#include <cassert>

SceneGameOver::SceneGameOver(SceneManager& manager,Stages playedStage):
	SceneBase(manager),
	_isSelectRestart(true),
	_playedStage(playedStage)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_gameOverHandle = LoadGraph("data/UI/GameOver.png");
	assert(_gameOverHandle != -1);

}

SceneGameOver::~SceneGameOver()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_gameOverHandle);
}

void SceneGameOver::Init()
{
}

void SceneGameOver::Update(Input& input)
{
	if (input.IsTriggered("up") || input.IsTriggered("down"))
	{
		_isSelectRestart = !_isSelectRestart;
	}
	if (input.IsTriggered("decision"))
	{
		if (_isSelectRestart)
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, _playedStage),FadeState::CircleFadeIn);
		}
		else
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
		}
	}
}

void SceneGameOver::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);
	DrawRotaGraph(screenW / 2, screenH / 2, 0.7, 0.0, _gameOverHandle, true);

	if (_isSelectRestart)
	{
		DrawString(screenW / 2, screenH / 2 + 300, "-> Restart\nBack to Stage Select",0xffffff);
	}
	else
	{
		DrawString(screenW / 2, screenH / 2 + 300, "Restart\n-> Back to Stage Select", 0xffffff);
	}
}