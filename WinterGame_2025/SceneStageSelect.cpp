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
	// 最初のステージと最後のステージ以外を選択中の時は選択中、左、右のステージ名を表示
	if (_selectIndex > 0 && _selectIndex < _stageList.size() - 1)
	{
		DrawFormatString(1920 / 2 - 400, 1080 / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
		DrawFormatString(1920 / 2, 1080 / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		DrawFormatString(1920 / 2 + 400, 1080 / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
	}
	else if (_selectIndex == 0)	// 最初のステージを選択中のときは選択中と右のステージ名を表示
	{
		DrawFormatString(1920 / 2, 1080 / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		DrawFormatString(1920 / 2 + 400, 1080 / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
	}
	else if (_selectIndex == _stageList.size() - 1)	// 最後のステージを選択中のときは選択中と左のステージ名を表示
	{
		DrawFormatString(1920 / 2 - 400, 1080 / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
		DrawFormatString(1920 / 2, 1080 / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
	}

#ifdef _DEBUG
	DrawString(0, 0, "SceneStageSelect",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "selectIndex:%d",_selectIndex);
#endif
}