#include "SceneTitle.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"
#include "SceneMain.h"
#include "../Game.h"
#include "../Application.h"
#include <cassert>
#include <string>
#include <cmath>

namespace
{
	constexpr int kTitleOffsetY = -200;
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

	if (input.IsTriggered("up"))
	{
		_selectIndex--;
		if (_selectIndex < 0)
		{
			_selectIndex = static_cast<int>(MenuItems::Num) - 1;
		}
	}
	if (input.IsTriggered("down"))
	{
		_selectIndex++;
		if (_selectIndex > static_cast<int>(MenuItems::Num) - 1)
		{
			_selectIndex = 0;
		}
	}
	if (input.IsTriggered("decision"))
	{
		// メニューごとの処理
		switch (static_cast<MenuItems>(_selectIndex))
		{
		case MenuItems::Start:	// スタート
			// 全てのステージが未クリアなら直接シーンメインへ
			if (_manager.GetSaveData().clearedStage == static_cast<int>(Stages::None))
			{
				_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Tutorial), FadeState::CircleFadeIn, FadeState::NormalFadeOut);
			}
			else	// そうでないならステージセレクトへ
			{
				_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
			}
			return;
		case MenuItems::Option:	// オプション
			// 現状オプションは未実装
			printfDx("オプションは未実装です\n");
			break;
		case MenuItems::Exit:	// 終了
			Application::GetInstance().RequestExit();
			break;
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
	float sin = sinf(_frame * 0.02f);
	DrawRotaGraph(screenW / 2, (screenH / 2 + sin * 15) + kTitleOffsetY, 1.0, 0.0, _titleHandle, true);
	
	// メニュー描画
	for (int i = 0; i < static_cast<int>(MenuItems::Num); i++)
	{
		int y = screenH / 2 + 170 + i * 130;
		// 未選択のメニューは暗く、小さく表示
		float scale = 0.8f;
		SetDrawBright(128, 128, 128);
		if (_selectIndex == i)	// 選択中のメニューは明るく、大きく表示
		{
			scale = 1.0f;
			SetDrawBright(255, 255, 255);
		}
		DrawRotaGraph(screenW / 2, y, scale, 0.0, _menuHandles[i], true);
		SetDrawBright(255, 255, 255);	// 明るさを元に戻す
	}
	//DrawRotaGraph(screenW / 2, screenH - 400, 1.0, 0.0, _startButtonH, true);
	//DrawRotaGraph(screenW / 2, screenH - 275, 1.0, 0.0, _optionButtonH, true);
	//DrawRotaGraph(screenW / 2, screenH - 150, 1.0, 0.0, _exitButtonH, true);

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}