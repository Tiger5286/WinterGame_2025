#include "SceneClear.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "Dxlib.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "DebugScene.h"
#include "../Game.h"
#include "../Utility/IntGraphDrawer.h"
#include <cassert>

namespace
{
	// クリアロゴ画像の位置とスケール
	constexpr int kClearLogoY = GlobalConstants::kScreenHeight / 2 - 100;
	constexpr float kClearLogoScale = 0.75f;
	// スコアテキスト画像の位置とスケール
	constexpr int kScoreTextX = GlobalConstants::kScreenWidth / 2 - 170;
	constexpr int kScoreTextY = GlobalConstants::kScreenHeight / 2 + 200;
	constexpr float kScoreTextScale = 0.7f;
	// スコアのカウントアップに使うLerpの値
	constexpr float kScoreCountUpLerp = 0.02f;
	// スコアの位置とスケール
	constexpr int kScoreX = GlobalConstants::kScreenWidth / 2 - 30;
	constexpr int kScoreY = GlobalConstants::kScreenHeight / 2 + 160;
	constexpr float kScoreScale = 0.7f;
	// ハイスコア更新画像の位置とスケールと角度
	constexpr int kHighScoreTextX = GlobalConstants::kScreenWidth / 2 + 300;
	constexpr int kHighScoreTextY = 150;
	constexpr float kHighScoreTextScale = 0.8f;
	constexpr float kHighScoreTextAngle = 0.2f;
	constexpr float kHighScoreTextSinValue = 0.1f;
	constexpr float kHighScoreTextSinScale = 0.3f;
	// 戻るテキスト画像の位置とスケール
	constexpr int kBackTextY = GlobalConstants::kScreenHeight / 2 + 350;
	constexpr float kBackTextScale = 0.5f;
	constexpr float kBackTextSinAlphaScale = 0.1f;	// 点滅に使うsinのスケール
}

SceneClear::SceneClear(SceneManager& manager,int score, Stages clearStage):
	SceneBase(manager),
	_score(score),
	_clearStage(clearStage)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_clearLogoHandle = LoadGraph("data/UI/StageClear.png");
	assert(_clearLogoHandle != -1);
	_scoreTextHandle = LoadGraph("data/UI/ScoreText.png");
	assert(_scoreTextHandle != -1);
	_numberTextHandle = LoadGraph("data/UI/NumberText.png");
	assert(_numberTextHandle != -1);
	_updateHighScoreHandle = LoadGraph("data/UI/UpdateHighScoreText.png");
	assert(_updateHighScoreHandle != -1);
	_backTextHandle = LoadGraph("data/UI/BackToStageSelectTextWithAButton.png");
	assert(_backTextHandle != -1);

	// ハイスコア更新
	auto selectableStage = StageToSelectableStage(clearStage);
	_isUpdateScore = manager.CheckHighScore(score, selectableStage);
	// クリア済みステージ登録
	manager.CheckClearedStage(selectableStage);

	// BGM再生
	SoundManager::GetInstance().LoadSound("ClearBgm", "data/Sounds/BGM/ClearBGM.ogg", SoundType::BGM);
	SoundManager::GetInstance().PlaySoundGame("ClearBgm", true, true);
}

SceneClear::~SceneClear()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_clearLogoHandle);
	DeleteGraph(_scoreTextHandle);
	DeleteGraph(_numberTextHandle);
	DeleteGraph(_updateHighScoreHandle);
	DeleteGraph(_backTextHandle);

	// bgmを解放
	SoundManager::GetInstance().DeleteSound("ClearBgm");
}

void SceneClear::Init()
{
}

void SceneClear::Update(Input& input)
{
	// フレームカウントを加算
	_frame++;

	// 決定を押したとき
	if (input.IsTriggered("decision"))
	{
		// スコア表示が最後まで行っていたらステージセレクトへ戻る
		if (_score == 0 || static_cast<int>(_dispScore) == _score - 1)
		{
			SoundManager::GetInstance().StopSound("ClearBgm", true);
			_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager, _clearStage));
		}
		else	// スコア表示が最後まで行っていなかったら最後まで進める
		{
			_dispScore = static_cast<float>(_score - 1);	// スコア表示を最後まで進める
		}
	}

#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneClear::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 背景の描画
	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);
	// クリア文字画像の描画
	DrawRotaGraph(screenW / 2, kClearLogoY, kClearLogoScale, 0.0, _clearLogoHandle, true);
	// スコア文字画像の描画
	DrawRotaGraph(kScoreTextX, kScoreTextY, kScoreTextScale, 0.0, _scoreTextHandle, true);

	// 描画用スコアの計算
	_dispScore = std::lerp(_dispScore, static_cast<float>(_score), kScoreCountUpLerp);
	// 描画用スコアを描画
	if (_score != 0)
	{
		IntGraphDrawer::Draw(kScoreX, kScoreY, kScoreScale,_numberTextHandle, static_cast<int>(_dispScore) + 1);// なぜか_score-1で止まるので+1して補正
	}
	else
	{
		IntGraphDrawer::Draw(kScoreX, kScoreY, kScoreScale, _numberTextHandle, static_cast<int>(_dispScore));// スコアが0のときに+1するとダメなのでそのまま表示
	}
	
	// ハイスコアを更新していたらそのメッセージを表示
	if (_isUpdateScore)
	{
		
		float sinScale = (sinf(_frame * kHighScoreTextSinValue) + 1) / 2;	// sinの値を-1~1 -> 0 ~ 1に加工
		sinScale *= kHighScoreTextSinScale;	// 表示したい大きさにスケーリング
		DrawRotaGraph(kHighScoreTextX, kHighScoreTextY, kHighScoreTextScale + sinScale, kHighScoreTextAngle, _updateHighScoreHandle, true);
	}

	// ステージセレクトへ戻るの描画
	// 透明度をsinで変える
	float sinAlpha = sinf(_frame * kBackTextSinAlphaScale) * 255 / 2;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, sinAlpha + 255 / 2);
	DrawRotaGraph(screenW / 2, kBackTextY, kBackTextScale, 0.0, _backTextHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DrawString(0, 0, "SceneClear", 0xffffff);
#endif
}
