#include "SceneTitle.h"
#include "Dxlib.h"
#include "Input.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Update(Input& input)
{
	if (input.IsTriggered("decision"))
	{
		_manager.ChangeScene(std::make_shared<SceneStageSelect>(_manager));
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
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawString(1920 / 2, 1080 / 2 + 250, "Press A to Start", 0xffffff);
}