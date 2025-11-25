#include "DebugScene.h"
#include "Input.h"
#include "Dxlib.h"
#include "SceneManager.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneStageSelect.h"

DebugScene::DebugScene(SceneManager& manager):
	SceneBase(manager),
	_selectIndex(0)
{
	_menuList = {
		"SceneMain",
		"SceneTitle",
		"SceneStageSelect"
	};
	_execTable["SceneMain"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager,Stages::Temp),FadeState::CircleFadeIn);
		};
	_execTable["SceneTitle"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneTitle>(_manager));
		};
	_execTable["SceneStageSelect"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneStageSelect>(_manager));
		};
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
}

void DebugScene::Update(Input& input)
{
	if (input.IsTriggered("up"))
	{
		_selectIndex = (_selectIndex + _menuList.size() - 1) % _menuList.size();
	}
	if (input.IsTriggered("down"))
	{
		_selectIndex = (_selectIndex + 1) % _menuList.size();
	}
	if (input.IsTriggered("decision"))
	{
		auto& menuName = _menuList[_selectIndex];
		_execTable[menuName]();
		return;
	}
}

void DebugScene::Draw()
{
	DrawBox(0, 0, 1920, 1080, 0x444444, true);

	DrawString(0, 0, "DebugScene", 0xffffff);
	for (int i = 0; i < _menuList.size(); i++)
	{
		if (i == _selectIndex)
		{
			DrawString(100, 100 + i * 30, ("-> " + _menuList[i]).c_str(), GetColor(255, 255, 0));
		}
		else
		{
			DrawString(100, 100 + i * 30, _menuList[i].c_str(), GetColor(255, 255, 255));
		}
	}
}
