#include "SceneStageSelect.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "Dxlib.h"
#include "DebugScene.h"

SceneStageSelect::SceneStageSelect(SceneManager& manager):
	SceneBase(manager),
	_selectIndex(0)
{
	_stageList =
	{
		"Tutorial Stage",
		"Stage 1",
		"Stage 2",
		"Boss Stage"
	};
	_execTable["Tutorial Stage"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager,Stages::Tutorial));
		};
	_execTable["Stage 1"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager, Stages::Stage1));
		};
	_execTable["Stage 2"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager, Stages::Stage2));
		};
	_execTable["Boss Stage"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager, Stages::Boss));
		};
}

SceneStageSelect::~SceneStageSelect()
{
}

void SceneStageSelect::Init()
{
}

void SceneStageSelect::Update(Input& input)
{
	if (input.IsTriggered("right"))
	{
		if (_selectIndex < _stageList.size() - 1)
		{
			_selectIndex++;
		}
	}
	if (input.IsTriggered("left"))
	{
		if (_selectIndex > 0)
		{
			_selectIndex--;
		}
	}
	if (input.IsTriggered("decision"))
	{
		auto& stageName = _stageList[_selectIndex];
		_execTable[stageName]();
		return;
	}

#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneStageSelect::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "SceneStageSelect",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "selectIndex:%d",_selectIndex);
#endif
}