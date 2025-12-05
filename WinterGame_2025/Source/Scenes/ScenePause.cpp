#include "ScenePause.h"
#include "SceneManager.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include "../Game.h"

#include "SceneStageSelect.h"

ScenePause::ScenePause(SceneManager& manager,Stages playingStage) :
	SceneBase(manager),
	_playingStage(playingStage)
{
	// メニュー項目
	_menuList =
	{
		"ゲームに戻る",
		"ステージセレクトに戻る"
	};

	// 各メニュー項目の実行内容
	_execTable["ゲームに戻る"] = [this]() {
		_manager.PopScene();
		return;
		};
	_execTable["ステージセレクトに戻る"] = [this]() {
		_manager.ResetSceneWithFade(std::make_shared<SceneStageSelect>(_manager, static_cast<int>(_playingStage) - 1));
		return;
		};
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
}

void ScenePause::Update(Input& input)
{
	if (input.IsTriggered("up"))
	{
		// メニューの選択肢を上に移動
		_selectIndex = (_selectIndex + _menuList.size() - 1) % _menuList.size();
	}
	if (input.IsTriggered("down"))
	{
		// メニューの選択肢を下に移動
		_selectIndex = (_selectIndex + 1) % _menuList.size();
	}
	if (input.IsTriggered("decision"))
	{
		// 選択中のメニュー項目を実行
		_execTable[_menuList[_selectIndex]]();
	}

	if (input.IsTriggered("start"))
	{
		_execTable["ゲームに戻る"]();
	}
}

void ScenePause::Draw()
{
	const int screenW = GlobalConstants::kScreenWidth;
	const int screenH = GlobalConstants::kScreenHeight;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	// 半透明にする
	DrawBox(0, 0, screenW, screenH, 0x000000, true);	// 黒い四角を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す

	DrawString(screenW / 2, 200, "pause", 0xffffff);

	for (int i = 0; i < _menuList.size(); i++)
	{
		unsigned int color = 0xffffff;
		if (i == _selectIndex)
		{
			color = 0x00ff00;	// 選択中のメニューは緑色にする
			DrawString(screenW / 2 - 20, 400 + i * 30, "→", color);
		}
		DrawString(screenW / 2, 400 + i * 30, _menuList[i].c_str(), color);
	}
}