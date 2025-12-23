#include "TutorialScene.h"
#include "SceneManager.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include <algorithm>

TutorialScene::TutorialScene(SceneManager& manager, TutorialStep nowStep):
	SceneBase(manager)
{
	switch (nowStep)
	{
	case TutorialStep::None:
		_tutorialText = "チュートリアルへようこそ！\n\nここでは基本操作を学びます。\n\nそれでは始めましょう！";
		break;
	case TutorialStep::Move:
		_tutorialText = "移動方法を学びましょう。\n\n十字ボタンの左右で移動します。";
		break;
	case TutorialStep::Shot:
		_tutorialText = "射撃方法を学びましょう。\n\nXボタンで通常ショットを撃ちます。";
		break;
	case TutorialStep::Jump:
		_tutorialText = "ジャンプ方法を学びましょう。\n\nAボタンでジャンプします。";
		break;
	case TutorialStep::ChargeShot:
		_tutorialText = "チャージショット方法を学びましょう。\n\nXボタンを長押しし、離すとチャージショットを撃ちます。";
		break;
	case TutorialStep::Dash:
		_tutorialText = "ダッシュ方法を学びましょう。\n\nBボタンでダッシュします。\n\nダッシュ中は空中でも水平に移動することができます。";
		break;
	case TutorialStep::WallJump:
		_tutorialText = "壁ジャンプ方法を学びましょう。\n\n空中で壁に向かって移動すると壁張り付き状態になり、\n\nその状態でジャンプすることで壁ジャンプができます。";
		break;
	case TutorialStep::DashInvincible:
		_tutorialText = "ダッシュ無敵を学びましょう。\n\nダッシュ中はあらゆるダメージを受けません。\n\n敵の攻撃やギミックの回避に活用しましょう。";
		break;
	case TutorialStep::JumpAndDash:
		_tutorialText = "ジャンプとダッシュの組み合わせを学びましょう。\n\nジャンプ中にダッシュをすることで、より遠くの足場まで移動できます。";
		break;
	case TutorialStep::Complete:
		_tutorialText = "チュートリアルは以上です！\n\n頑張ってステージクリアを目指しましょう！";
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

	DrawString(400, 400, _tutorialText.c_str(), 0xffffff);
}