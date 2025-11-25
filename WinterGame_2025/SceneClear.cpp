#include "SceneClear.h"
#include "Input.h"
#include "Dxlib.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "DebugScene.h"

SceneClear::SceneClear(SceneManager& manager):
	SceneBase(manager)
{
}

SceneClear::~SceneClear()
{
}

void SceneClear::Init()
{
}

void SceneClear::Update(Input& input)
{
	if (input.IsTriggered("decision"))
	{
		_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
	}

#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneClear::Draw()
{
	DrawBox(0, 0, 1920, 1080, 0x444444, true);

	DrawString(1920 / 2, 1080 / 2 , "Stage Clear", 0xffffff);
	DrawString(1920 / 2, 1080 / 2 + 250, "Press A to Back to Stage Select", 0xffffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneClear", 0xffffff);
#endif
}
