#include "SceneClear.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "Dxlib.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "DebugScene.h"
#include "../Game.h"
#include <cassert>

SceneClear::SceneClear(SceneManager& manager,int score, Stages clearStage):
	SceneBase(manager),
	_score(score),
	_clearStage(clearStage)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_clearLogoHandle = LoadGraph("data/UI/StageClear.png");
	assert(_clearLogoHandle != -1);

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

	// bgmを解放
	SoundManager::GetInstance().DeleteSound("ClearBgm");
}

void SceneClear::Init()
{
}

void SceneClear::Update(Input& input)
{
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
			_dispScore = _score - 1;	// スコア表示を最後まで進める
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
	DrawRotaGraph(screenW / 2, screenH / 2 - 100, 0.75, 0.0, _clearLogoHandle, true);

	// 描画用スコアの計算
	_dispScore = std::lerp(_dispScore, static_cast<float>(_score), 0.02f);
	// 描画用スコアを描画
	if (_score != 0)
	{
		DrawFormatString(screenW / 2, screenH / 2 + 130, 0xffffff, "Score:%d", static_cast<int>(_dispScore) + 1);	// なぜか_score-1で止まるので+1して補正
	}
	else
	{
		DrawFormatString(screenW / 2, screenH / 2 + 130, 0xffffff, "Score:%d", static_cast<int>(_dispScore));	// スコアが0のときに+1するとダメなのでそのまま表示
	}
	
	// ハイスコアを更新していたらそのメッセージを表示
	if (_isUpdateScore)
	{
		DrawString(screenW / 2, 150, "High score update!", 0xffffff);
	}

	DrawString(screenW / 2, screenH / 2 + 250, "Press A to Back to Stage Select", 0xffffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneClear", 0xffffff);
#endif
}
