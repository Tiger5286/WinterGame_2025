#include "TutorialScene.h"
#include "SceneManager.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kFontSize = 32;
}

TutorialScene::TutorialScene(SceneManager& manager, TutorialStep nowStep):
	SceneBase(manager)
{
	_fontHandle = CreateFontToHandle("廻想体 ネクスト UP B", kFontSize, -1);
	assert(_fontHandle != -1 && "フォントの生成に失敗しました");

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
	if (input.IsTriggered("decision"))
	{	// メインシーンへ切り替え
		_manager.PopScene();
		return;
	}
}

void TutorialScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(380, 380, 1000, 600, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(400, 400, _tutorialText.c_str(), 0xffffff,_fontHandle);
	unsigned int color = 0xffff00;
	if (_frame % 30 < 15)
	{
		color = 0xffffff;
	}
	DrawStringToHandle(850, 570, "Aボタンで進む", color,_fontHandle);
}