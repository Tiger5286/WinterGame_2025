#include "SceneGameOver.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "../Game.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"

#include <cassert>

namespace
{
	constexpr float kGameOverGraphScale = 0.7f;
	constexpr int kGameOverGraphY = GlobalConstants::kScreenHeight / 2 - 100;

	constexpr float kSelectMenuScale = 0.6f;
	constexpr float kUnselectMenuScale = 0.4f;

	constexpr int kRestartTextY = GlobalConstants::kScreenHeight / 2 + 240;
	constexpr int kBackTextY = GlobalConstants::kScreenHeight / 2 + 350;

	constexpr float kMenuSinScale = 0.05f;
}

SceneGameOver::SceneGameOver(SceneManager& manager,Stages playedStage,int score):
	SceneBase(manager),
	_isSelectRestart(true),
	_score(score),
	_playedStage(playedStage)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_gameOverHandle = LoadGraph("data/UI/GameOver.png");
	assert(_gameOverHandle != -1);
	_restartTextHandle = LoadGraph("data/UI/RestartText.png");
	assert(_restartTextHandle != -1);
	_backTextHandle = LoadGraph("data/UI/BackToStageSelectText.png");
	assert(_backTextHandle != -1);

	SoundManager::GetInstance().PlaySoundGame("GameOverBgm", true, true);
}

SceneGameOver::~SceneGameOver()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_gameOverHandle);
	DeleteGraph(_restartTextHandle);
	DeleteGraph(_backTextHandle);
}

void SceneGameOver::Init()
{
}

void SceneGameOver::Update(Input& input)
{
	_frame++;
	// 上下を押して選択しているメニューを変える
	if (input.IsTriggered("up") || input.IsTriggered("down"))
	{
		SoundManager::GetInstance().PlaySoundGame("Select");
		_isSelectRestart = !_isSelectRestart;
	}
	// 決定ボタンを押したとき
	if (input.IsTriggered("decision"))
	{
		SoundManager::GetInstance().StopSound("GameOverBgm", true);
		SoundManager::GetInstance().PlaySoundGame("Decision");
		if (_isSelectRestart)	// リスタートを押したとき
		{
			if (IsBossStage(_playedStage))
			{
				// ボスステージならスコアを引き継ぐ
				_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, _playedStage, _score), FadeState::CircleFadeIn);
				return;
			}
			else
			{
				// それ以外のステージならスコアは0に戻す
				_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, _playedStage, 0), FadeState::CircleFadeIn);
				return;
			}
			
		}
		else	// 戻るを押したとき
		{
			auto selectableStage = StageToSelectableStage(_playedStage);
			_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager, _playedStage));
		}
	}
}

void SceneGameOver::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 背景の描画
	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);
	// ロゴの描画
	DrawRotaGraph(screenW / 2, kGameOverGraphY, kGameOverGraphScale, 0.0, _gameOverHandle, true);

	// メニューの描画
	float sinScale = sinf(_frame * kMenuSinScale) * kMenuSinScale;
	if (_isSelectRestart)
	{
		// 上
		DrawRotaGraph(screenW / 2, kRestartTextY, kSelectMenuScale + sinScale, 0.0, _restartTextHandle, true);
		// 下
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawRotaGraph(screenW / 2, kBackTextY, kUnselectMenuScale, 0.0, _backTextHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// 上
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawRotaGraph(screenW / 2, kRestartTextY, kUnselectMenuScale, 0.0, _restartTextHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 下
		DrawRotaGraph(screenW / 2, kBackTextY, kSelectMenuScale + sinScale, 0.0, _backTextHandle, true);
	}
}