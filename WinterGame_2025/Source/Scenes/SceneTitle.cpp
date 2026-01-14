#include "SceneTitle.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "../Game.h"
#include "../Application.h"
#include "../Systems/SoundManager.h"
#include <cassert>
#include <string>
#include <cmath>

namespace
{
	// タイトルUI関連
	constexpr int kTitlePosY = GlobalConstants::kScreenHeight / 2 - 200;
	constexpr float kTitleSinRate = 0.02f;
	constexpr float kTitleSinScale = 15.0f;

	// メニューUI関連
	constexpr int kUiPosYTop = GlobalConstants::kScreenHeight / 2 + 100;
	constexpr int kUiPosYInterval = 150;
	constexpr float kUiScaleSelected = 0.5f;
	constexpr float kUiScaleUnselected = 0.4f;
	constexpr float kUiSinRate = 0.05f;
	constexpr float kUiSinScale = 0.05f;

	// スタート選択後の待機フレーム数
	constexpr int kStartWaitFrame = 20;
}

enum class MenuItems
{
	Start,
	Option,
	Exit,
	Num
};

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_titleHandle = LoadGraph("data/UI/Title.png");
	assert(_titleHandle != -1);
	_menuHandles[static_cast<int>(MenuItems::Start)] = LoadGraph("data/UI/StartButton.png");
	_menuHandles[static_cast<int>(MenuItems::Option)] = LoadGraph("data/UI/OptionButton.png");
	_menuHandles[static_cast<int>(MenuItems::Exit)] = LoadGraph("data/UI/ExitButton.png");
	for (const auto& handle : _menuHandles)
	{
		assert(handle != -1);
	}

	_fontHandle = CreateFontToHandle(GlobalConstants::kMainFontName, 64, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	SoundManager::GetInstance().PlaySoundGame("TitleBGM", true,true);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_titleHandle);
	for (const auto& handle : _menuHandles)
	{
		DeleteGraph(handle);
	}

	DeleteFontToHandle(_fontHandle);
}

void SceneTitle::Init()
{
}

void SceneTitle::Update(Input& input)
{
	_frame++;

	if (input.IsTriggered("up") && !_isTriggeredStart)
	{
		_selectIndex--;
		if (_selectIndex < 0)
		{
			_selectIndex = static_cast<int>(MenuItems::Num) - 1;
		}
		SoundManager::GetInstance().PlaySoundGame("Select");
	}
	if (input.IsTriggered("down") && !_isTriggeredStart)
	{
		_selectIndex++;
		if (_selectIndex > static_cast<int>(MenuItems::Num) - 1)
		{
			_selectIndex = 0;
		}
		SoundManager::GetInstance().PlaySoundGame("Select");
	}
	if (input.IsTriggered("decision") && !_isTriggeredStart)
	{
		SoundManager::GetInstance().PlaySoundGame("Decision");
		// メニューごとの処理
		switch (static_cast<MenuItems>(_selectIndex))
		{
		case MenuItems::Start:	// スタート
			_isTriggeredStart = true;
			break;
		case MenuItems::Option:	// オプション
			// オプションシーンをプッシュ
			_manager.PushScene(std::make_shared<SceneOption>(_manager));
			break;
		case MenuItems::Exit:	// 終了
			// アプリケーション終了をリクエスト
			Application::GetInstance().RequestExit();
			break;
		}
	}

	// スタートが選択されてから実行される処理
	if (_isTriggeredStart)
	{
		// 少し待機してからシーン遷移
		_frameCount++;
		if (_frameCount > kStartWaitFrame)
		{
			OnTriggeredStart();
			return;
		}
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
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 背景描画
	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);

	// タイトル描画
	float sin = sinf(_frame * kTitleSinRate);
	DrawRotaGraph(screenW / 2, static_cast<int>(sin * kTitleSinScale) + kTitlePosY, 1.0, 0.0, _titleHandle, true);
	
	// メニュー描画
	for (int i = 0; i < static_cast<int>(MenuItems::Num); i++)
	{
		int y = kUiPosYTop + i * kUiPosYInterval;
		// 未選択のメニューは暗く、小さく表示
		float scale = kUiScaleUnselected;
		SetDrawBright(128, 128, 128);
		if (_selectIndex == i)	// 選択中のメニュー
		{
			scale = kUiScaleSelected + kUiSinScale * sinf(_frame * kUiSinRate);	// 選択中のメニューを少し拡大縮小させる
			SetDrawBright(255, 255, 255);	// 選択中のメニューは明るく表示
			// 選択確定後は点滅させる
			if (_frameCount % 4 >= 2)
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 0);	// 選択中のメニューを点滅させる
			}
		}
		DrawRotaGraph(screenW / 2, y, scale, 0.0, _menuHandles[i], true);
		SetDrawBright(255, 255, 255);	// 明るさを元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す
	}

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}

void SceneTitle::OnTriggeredStart()
{
	SoundManager::GetInstance().StopSound("TitleBGM", true);	// BGM停止
	// 全てのステージが未クリアなら直接シーンメインへ
	if (_manager.GetSaveData().clearedStage == static_cast<int>(Stages::None))
	{
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Tutorial), FadeState::CircleFadeIn, FadeState::NormalFadeOut);
	}
	else	// そうでないならステージセレクトへ
	{
		_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
	}
}
