#include "DebugScene.h"
#include "Input.h"
#include "Dxlib.h"
#include "SceneManager.h"

#include "SceneMain.h"
#include "SceneTitle.h"

DebugScene::DebugScene(SceneManager& manager):
	SceneBase(manager),
	_selectIndex(0)
{
	_menuList = {
		"SceneMain",
		"SceneTitle"
	};
	_execTable["SceneMain"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager));
		};
	_execTable["SceneTitle"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneTitle>(_manager));
		};
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
}

void DebugScene::Update(Input input)
{
	if (input.IsTriggered("up"))
	{
		_selectIndex = (_selectIndex + _menuList.size() - 1) % _menuList.size();
	}
	if (input.IsTriggered("down"))
	{
		_selectIndex = (_selectIndex + 1) % _menuList.size();
	}
	if (input.IsTriggered("select"))
	{
		auto& menuName = _menuList[_selectIndex];
		_execTable[menuName]();
		return;
	}
}

void DebugScene::Draw()
{
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
