#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneOption.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "Dxlib.h"
#include "../Game.h"
#include <cassert>

#include "SceneStageSelect.h"

namespace
{
	// ウィンドウの端が画面端からどれくらい離れているか
	constexpr int kWindowMargin = 150;

	// フォントサイズ
	constexpr int kFontSize = 64;

	// テキストのY座標
	constexpr int kPauseTextY = 200;
	constexpr int kMenuStartY = 400;
	// メニュー間のY座標の間隔
	constexpr int kMenuMarginY = 40;

	constexpr int kSinScale = 10;
}

ScenePause::ScenePause(SceneManager& manager,Stages playingStage) :
	SceneBase(manager),
	_playingStage(playingStage)
{
	// フォントの生成
	_fontHandle = CreateFontToHandle("廻想体 ネクスト UP B", kFontSize, -1);
	assert(_fontHandle != -1 && "フォントの生成に失敗しました");
	// 画像のロード
	_bgHandle = LoadGraph("data/UI/PauseBg.png");
	assert(_bgHandle != -1 && "背景画像のロードに失敗しました");

	// メニュー項目
	_menuList =
	{
		"ゲームに戻る",
		"オプション",
		"ステージセレクトに戻る"
	};

	// 各メニュー項目の実行内容
	_execTable["ゲームに戻る"] = [this]() {
		SoundManager::GetInstance().PlaySoundGame("Cancel");
		_manager.PopScene();
		return;
		};
	_execTable["オプション"] = [this]() {
		SoundManager::GetInstance().PlaySoundGame("Decision");
		_manager.PushScene(std::make_shared<SceneOption>(_manager));
		return;
		};
	_execTable["ステージセレクトに戻る"] = [this]() {
		SoundManager::GetInstance().PlaySoundGame("Decision");
		SoundManager::GetInstance().StopSoundAll(true);
		_manager.ResetSceneWithFade(std::make_shared<SceneStageSelect>(_manager, _playingStage));
		return;
		};
}

ScenePause::~ScenePause()
{
	DeleteFontToHandle(_fontHandle);
	DeleteGraph(_bgHandle);
}

void ScenePause::Init()
{
}

void ScenePause::Update(Input& input)
{
	_frame++;
	if (input.IsTriggered("up"))
	{
		// メニューの選択肢を上に移動
		_selectIndex = (_selectIndex + _menuList.size() - 1) % static_cast<int>(_menuList.size());
		SoundManager::GetInstance().PlaySoundGame("Select");
	}
	if (input.IsTriggered("down"))
	{
		// メニューの選択肢を下に移動
		_selectIndex = (_selectIndex + 1) % _menuList.size();
		SoundManager::GetInstance().PlaySoundGame("Select");
	}
	if (input.IsTriggered("decision"))
	{
		// 選択中のメニュー項目を実行
		_execTable[_menuList[_selectIndex]]();
	}
	if (input.IsTriggered("back") || input.IsTriggered("start"))
	{
		_execTable["ゲームに戻る"]();
		SoundManager::GetInstance().PlaySoundGame("Cancel");
	}
}

void ScenePause::Draw()
{
	const int screenW = GlobalConstants::kScreenWidth;
	const int screenH = GlobalConstants::kScreenHeight;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	// 半透明にする
	DrawBox(0,0, screenW, screenH, 0x000000, true);	// 黒い四角を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す
	//DrawBox(kBlackBoxMargin, kBlackBoxMargin, screenW - kBlackBoxMargin, screenH - kBlackBoxMargin, 0xffffff, false,5);	// 黒い四角を描画

	DrawExtendGraph(kWindowMargin, kWindowMargin, screenW - kWindowMargin, screenH - kWindowMargin, _bgHandle, true);

	std::string pauseText = "ポーズ";
	int pauseTextWidth = GetDrawStringWidthToHandle(pauseText.c_str(), static_cast<int>(pauseText.length()), _fontHandle);
	DrawStringToHandle(screenW / 2 - pauseTextWidth / 2, kPauseTextY, pauseText.c_str(), 0xffffff, _fontHandle);

	for (int i = 0; i < _menuList.size(); i++)
	{
		int menuTextWidth = GetDrawStringWidthToHandle(_menuList[i].c_str(), static_cast<int>(_menuList[i].length()), _fontHandle);
		unsigned int color = 0xffffff;
		if (i == _selectIndex)
		{
			float sin = sinf(_frame * 0.1f) * 0.5f + 0.5f;	// 0~1の間を往復する
			color = 0x33dd33;	// 選択中のメニューは緑色にする
			DrawStringToHandle(screenW / 2 - menuTextWidth / 2 - kFontSize - static_cast<int>(sin * kSinScale), kMenuStartY + i * (kFontSize + kMenuMarginY), "→", color, _fontHandle);
		}
		DrawStringToHandle(screenW / 2 - menuTextWidth / 2, kMenuStartY + i * (kFontSize + kMenuMarginY), _menuList[i].c_str(), color, _fontHandle);
	}
}