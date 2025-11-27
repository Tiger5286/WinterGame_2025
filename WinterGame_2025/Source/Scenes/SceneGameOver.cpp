#include "SceneGameOver.h"
#include "Dxlib.h"
#include "../Systems/Input.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"

SceneGameOver::SceneGameOver(SceneManager& manager,Stages playedStage):
	SceneBase(manager),
	_isSelectRestart(true),
	_playedStage(playedStage)
{
}

SceneGameOver::~SceneGameOver()
{
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
	DrawBox(0, 0, 1920, 1080, 0x444444, true);

	DrawString(1920 / 2, 1080 / 2, "GameOver", 0xffffff);
	if (_isSelectRestart)
	{
		DrawString(1920 / 2, 1080 / 2 + 300, "-> Restart\nBack to Stage Select",0xffffff);
	}
	else
	{
		DrawString(1920 / 2, 1080 / 2 + 300, "Restart\n-> Back to Stage Select", 0xffffff);
	}
}