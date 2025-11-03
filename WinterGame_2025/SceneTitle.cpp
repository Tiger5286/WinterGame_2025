#include "SceneTitle.h"
#include "Dxlib.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneMain.h"

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

void SceneTitle::Update(Input input)
{
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager));
	}
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);
}