#include "DebugScene.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include "SceneManager.h"
#include "../Systems/DataManager.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneStageSelect.h"
#include "SceneClear.h"
#include "SceneGameOver.h"

#include <cassert>

#include "../Utility/IntGraphDrawer.h"

DebugScene::DebugScene(SceneManager& manager):
	SceneBase(manager),
	_selectIndex(0)
{
	_menuList = {
		"SceneMain",
		"SceneTitle",
		"SceneStageSelect",
		"SceneClear",
		"SceneGameOver"
	};
	_execTable["SceneMain"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneMain>(_manager,Stages::SecretStage),FadeState::CircleFadeIn);
		};
	_execTable["SceneTitle"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneTitle>(_manager));
		};
	_execTable["SceneStageSelect"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneStageSelect>(_manager));
		};
	_execTable["SceneClear"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneClear>(_manager, 12345, Stages::Tutorial));
		};
	_execTable["SceneGameOver"] = [this]() {
		_manager.ChangeScene(std::make_shared<SceneGameOver>(_manager, Stages::Tutorial, 0));
		};

	_tempFontHandle = CreateFontToHandle(GlobalConstants::kMainFontName, 48, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(_tempFontHandle != -1);
	_numberTextHandle = LoadGraph("data/UI/NumberText.png");
	assert(_numberTextHandle != -1);
}

DebugScene::~DebugScene()
{
	DeleteFontToHandle(_tempFontHandle);
	DeleteGraph(_numberTextHandle);
}

void DebugScene::Init()
{
}

void DebugScene::Update(Input& input)
{
	if (input.IsTriggered("up"))
	{
		_selectIndex = (_selectIndex + _menuList.size() - 1) % static_cast<int>(_menuList.size());
	}
	if (input.IsTriggered("down"))
	{
		_selectIndex = (_selectIndex + 1) % _menuList.size();
	}
	if (input.IsTriggered("decision"))
	{
		clsDx();
		auto& menuName = _menuList[_selectIndex];
		_execTable[menuName]();
		return;
	}

	if (input.IsTriggered("select"))
	{
		_manager._saveData.clearedStage = static_cast<int>(Stages::None);
		_manager._saveData.highScores.fill(0);
		_manager._saveData.isReleasedSecretStage = false;

		_manager._pDataManager->DeleteSaveData();
		printfDx("Delete SaveData\n");
	}
	if (input.IsTriggered("start"))
	{
		_manager._saveData.clearedStage = static_cast<int>(SelectableStages::Num) - 1;
		printfDx("All Stages Cleared\n");
	}
}

void DebugScene::Draw()
{
	// ”wŒi‚Ì•`‰æ
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

	DrawStringToHandle(300, 300, "ƒeƒXƒg—p•¶Žš—ñ‚Å‚·", 0x000000, _tempFontHandle, 0xffffff);

	IntGraphDrawer::Draw(100, 700, 0.8f, _numberTextHandle, 9358484);
}
