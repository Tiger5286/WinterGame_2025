#include "Application.h"
#include "DxLib.h"
#include "Game.h"
#include "Systems/SoundManager.h"
#include "Systems/input.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneTitle.h"
#include "Scenes/DebugScene.h"
#include <Windows.h>

Application::Application()
{
}

Application::~Application()
{
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
	ChangeWindowMode(false);
#ifdef _DEBUG
	ChangeWindowMode(true);
#endif
	SetMainWindowText("ブルロボ"); // ウインドウのタイトルを設定
	SetGraphMode(GlobalConstants::kScreenWidth, GlobalConstants::kScreenHeight, GlobalConstants::kColorBitNum); // 画面サイズと色数を設定
	if (DxLib_Init() == -1)
	{
		return false;
	}
	return true;
}

void Application::Run()
{
	SetDrawScreen(DX_SCREEN_BACK);

	// フォントを読み込む
	AddFontResourceExA("data/UI/MainFont.ttf", FR_PRIVATE, NULL);

	Input input;
	SceneManager sceneManager;
	SoundManager::GetInstance();
#ifdef _DEBUG
	sceneManager.ChangeScene(std::make_shared<DebugScene>(sceneManager));
#else
	sceneManager.ChangeScene(std::make_shared<SceneTitle>(sceneManager));
#endif

	while (ProcessMessage() != -1 && !_isRequestExit)
	{
		auto frameStartTime = GetNowHiPerformanceCount(); // フレーム開始時間を取得
		ClearDrawScreen(); // 画面をクリア

		input.Update();
		sceneManager.Update(input);
		SoundManager::GetInstance().Update();
		sceneManager.Draw();

		// escキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
		ScreenFlip(); // 描画した内容を画面に反映する
		while (GetNowHiPerformanceCount() - frameStartTime < 16667) {} // 約16.667ミリ秒(1/60秒)待つことで60FPSに固定
	}
}

void Application::Terminate()
{
	// 読み込んだフォントを解放
	RemoveFontResourceExA("data/UI/MainFont.ttf", FR_PRIVATE, NULL);

	DxLib_End();				// ＤＸライブラリ使用の終了処理
}