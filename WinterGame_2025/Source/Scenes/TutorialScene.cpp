#include "TutorialScene.h"
#include "SceneManager.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kFontSize = 32;

	// Aボタンが効かないフレーム数
	constexpr int kInputWaitFrame = 45;


	constexpr int kFrameLeftPos = 380;
	constexpr int kFrameTopPos = 380;
	constexpr int kFrameRightPos = 1000;
	constexpr int kFrameBottomPos = 600;

	constexpr int kTextPosX = 400;
	constexpr int kTextPosY = 400;

	constexpr int kButtonUIPosX = 920;
	constexpr int kButtonUIPosY = 530;

	constexpr int kButtonFlickerFrame = 30;
}

TutorialScene::TutorialScene(SceneManager& manager, TutorialStep nowStep):
	SceneBase(manager)
{
	_fontHandle = CreateFontToHandle("廻想体 ネクスト UP B", kFontSize, -1);
	assert(_fontHandle != -1 && "フォントの生成に失敗しました");
	_buttonHandle = LoadGraph("data/UI/Buttons/button_a.png");
	assert(_buttonHandle != -1 && "ボタン画像の読み込みに失敗しました");
	_buttonOutlineHandle = LoadGraph("data/UI/Buttons/button_a_outline.png");
	assert(_buttonOutlineHandle != -1 && "ボタン画像の読み込みに失敗しました");

	switch (nowStep)
	{
	case TutorialStep::None:
		_tutorialText = "None";
		break;
	case TutorialStep::Move:
		_tutorialText = "チュートリアルへようこそ！\n\nまずは移動だ！";
		break;
	case TutorialStep::Shot:
		_tutorialText = "射撃して敵を倒せ！";
		break;
	case TutorialStep::Jump:
		_tutorialText = "ジャンプで段差を乗り越えろ！";
		break;
	case TutorialStep::ChargeShot:
		_tutorialText = "チャージショットは敵を貫通する！";
		break;
	case TutorialStep::Dash:
		_tutorialText = "ダッシュをすれば空中でも落ちずに進める！";
		break;
	case TutorialStep::WallJump:
		_tutorialText = "壁ジャンプで高いところまで登れる！";
		break;
	case TutorialStep::DashInvincible:
		_tutorialText = "ダッシュ中はダメージを受けない！";
		break;
	case TutorialStep::JumpAndDash:
		_tutorialText = "アクションを駆使して大穴を飛び越えろ！";
		break;
	case TutorialStep::Complete:
		_tutorialText = "ワープゲートに触れたらボス戦だ！\n\n目指せ全ステージクリア！";
		break;
	}
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Init()
{
}

void TutorialScene::Update(Input& input)
{
	_frame++;
	if (input.IsTriggered("decision") && _frame > kInputWaitFrame)
	{	// メインシーンへ切り替え
		_manager.PopScene();
		return;
	}
}

void TutorialScene::Draw()
{
	// 背景の半透明黒枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(kFrameLeftPos, kFrameTopPos, kFrameRightPos, kFrameBottomPos, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 枠線描画
	DrawBox(kFrameLeftPos, kFrameTopPos, kFrameRightPos, kFrameBottomPos, 0xffffff, false,5);

	// テキスト描画
	DrawStringToHandle(kTextPosX, kTextPosY, _tutorialText.c_str(), 0xffffff,_fontHandle);
	// AボタンUI描画
	if (_frame % kButtonFlickerFrame < kButtonFlickerFrame / 2)
	{
		DrawGraph(kButtonUIPosX, kButtonUIPosY, _buttonHandle, true);
	}
	else
	{
		DrawGraph(kButtonUIPosX, kButtonUIPosY, _buttonOutlineHandle, true);
	}
}