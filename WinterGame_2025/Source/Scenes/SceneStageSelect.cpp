#include "SceneStageSelect.h"
#include "../Systems/Input.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Dxlib.h"
#include "DebugScene.h"
#include "../Game.h"
#include "../Systems/SoundManager.h"
#include "../Utility/IntGraphDrawer.h"
#include <cassert>

namespace
{
	constexpr int kUIControllInterval = 10;
	constexpr int kUIMoveScale = 40;
	constexpr float kUIDrawScale = 0.5f;
	constexpr float kUIDrawScaleHalf = kUIDrawScale / 2;
}

SceneStageSelect::SceneStageSelect(SceneManager& manager, Stages playedStage):
	SceneBase(manager),
	_frame(kUIControllInterval)
{
	// 画像のロード
	_bgHandle = LoadGraph("data/map/bg.png");
	assert(_bgHandle != -1);
	_emptyStageUIHandle = LoadGraph("data/UI/StageUI.png");
	assert(_emptyStageUIHandle != -1);

	// ステージUIの画像ロード
	std::vector<std::string> stageUIFileNames;
	stageUIFileNames.push_back("data/UI/TutorialStageUI.png");
	stageUIFileNames.push_back("data/UI/Stage1UI.png");
	stageUIFileNames.push_back("data/UI/Stage2UI.png");
	stageUIFileNames.push_back("data/UI/Stage3UI.png");
	stageUIFileNames.push_back("data/UI/SecretStageUI.png");
	for (auto& fileName : stageUIFileNames)
	{
		_stageUIHandles.push_back(LoadGraph(fileName.c_str()));
		assert(_stageUIHandles.back() != -1);
	}

	// ステージ影UI画像ロード
	std::vector<std::string> stageShadowUIFileNames;
	stageShadowUIFileNames.push_back("data/UI/TutorialStageShadowUI.png");
	stageShadowUIFileNames.push_back("data/UI/Stage1ShadowUI.png");
	stageShadowUIFileNames.push_back("data/UI/Stage2ShadowUI.png");
	stageShadowUIFileNames.push_back("data/UI/Stage3ShadowUI.png");
	stageShadowUIFileNames.push_back("data/UI/SecretStageShadowUI.png");
	for (auto& fileName : stageShadowUIFileNames)
	{
		_stageShadowUIHandles.push_back(LoadGraph(fileName.c_str()));
		assert(_stageShadowUIHandles.back() != -1);
	}

	// ステージ名画像ロード
	std::vector<std::string> stageNameFileNames;
	stageNameFileNames.push_back("data/UI/TutorialStageName.png");
	stageNameFileNames.push_back("data/UI/Stage1Name.png");
	stageNameFileNames.push_back("data/UI/Stage2Name.png");
	stageNameFileNames.push_back("data/UI/Stage3Name.png");
	stageNameFileNames.push_back("data/UI/SecretStageName.png");
	for (auto& fileName : stageNameFileNames)
	{
		_stageNameHandles.push_back(LoadGraph(fileName.c_str()));
		assert(_stageNameHandles.back() != -1);
	}

	// 数字画像のロード
	_numberGraphHandle = LoadGraph("data/UI/NumberText.png");
	// ハイスコアのテキスト画像ロード
	_highScoreTextHandle = LoadGraph("data/UI/HighScoreText.png");

	auto selectableStage = StageToSelectableStage(playedStage);

	// 選択位置をプレイしたステージに合わせる
	if (selectableStage == SelectableStages::None)
	{
		printfDx("playedStageがNoneになっています");
		_selectIndex = 0;
	}
	else
	{
		_selectIndex = static_cast<int>(selectableStage) - 1;
	}

	// ステージリスト
	_stageList =
	{
		"Tutorial Stage",
		"Stage 1",
		"Stage 2",
		"Stage 3",
		"Secret Stage"
	};
	// ステージ選択時の処理
	_execTable["Tutorial Stage"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager,Stages::Tutorial),FadeState::CircleFadeIn);
		};
	_execTable["Stage 1"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Stage1), FadeState::CircleFadeIn);
		};
	_execTable["Stage 2"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Stage2), FadeState::CircleFadeIn);
		};
	_execTable["Stage 3"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Stage3), FadeState::CircleFadeIn);
		};
	_execTable["Secret Stage"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::SecretStage), FadeState::CircleFadeIn);
		};

	// 最後のステージを選択している場合、右に存在しない次のステージが見えるのを防ぐ
	const bool isSelectLast = (_selectIndex == static_cast<int>(SelectableStages::Num) - 2);
	const bool isSelectPenultimate = (_selectIndex == static_cast<int>(SelectableStages::Num) - 3);
	if (isSelectLast ||
		isSelectPenultimate && !manager.GetSaveData().isReleasedSecretStage)
	{
		_isUIMoveRight = false;
		_frame = -kUIControllInterval;
	}

	// bgmのロードと再生
	SoundManager::GetInstance().LoadSound("StageSelectBGM", "data/Sounds/BGM/StageSelectBGM.ogg", SoundType::BGM);
	SoundManager::GetInstance().PlaySoundGame("StageSelectBGM", true, true);
}

SceneStageSelect::~SceneStageSelect()
{
	// リソースの解放
	DeleteGraph(_bgHandle);
	DeleteGraph(_emptyStageUIHandle);
	DeleteGraph(_numberGraphHandle);
	DeleteGraph(_highScoreTextHandle);
	for (auto& handle : _stageUIHandles)
	{
		DeleteGraph(handle);
	}
	for (auto& handle : _stageNameHandles)
	{
		DeleteGraph(handle);
	}
	SoundManager::GetInstance().DeleteSound("StageSelectBGM");
}

void SceneStageSelect::Init()
{
}

void SceneStageSelect::Update(Input& input)
{
	// UI移動演出用フレームカウントの更新
	if (_frame < kUIControllInterval && _isUIMoveRight)
	{	// 右移動中
		_frame++;
	}
	if (_frame > -kUIControllInterval && !_isUIMoveRight)
	{	// 左移動中
		_frame--;
	}

	// UI移動演出中は操作が効かないようにする
	if (_frame == kUIControllInterval || _frame == -kUIControllInterval)
	{
		if (input.IsTriggered("right"))
		{
			// 最後のステージ以外を選択中の時のみ右に移動可能
			if (_selectIndex < _stageList.size() - 1 &&
				_selectIndex < _manager.GetSaveData().clearedStage)	// クリア済みステージの次までしか移動できないようにする
			{
				// 隠しステージが解放されている場合、最後のステージに移動可能にする
				if (_selectIndex == static_cast<int>(SelectableStages::Num) - 3)	// 隠しステージの一つ手前 = Num - 3
				{	// 隠しステージの一つ手前を選択中の時、隠しステージが解放されていれば移動する
					if (_manager.GetSaveData().isReleasedSecretStage)
					{
						_selectIndex++;
						_isUIMoveRight = false;
						_frame = 0;
					}
					else	// 隠しステージが解放されていない場合、右入力カウントを増やす
					{
						_inputRightCount++;
						if (_inputRightCount >= 15)	// 15回右入力したら隠しステージ解放
						{
							_manager.ReleaseSecretStage();
						}
					}
				}
				else // 通常のステージ移動
				{
					_selectIndex++;
					_isUIMoveRight = false;
					_frame = 0;
				}
			}
		}
		if (input.IsTriggered("left"))
		{
			if (_selectIndex > 0)
			{
				_selectIndex--;
				_isUIMoveRight = true;
				_frame = 0;
			}
		}
		if (input.IsTriggered("decision"))
		{
			SoundManager::GetInstance().StopSound("StageSelectBGM", true);
			auto& stageName = _stageList[_selectIndex];
			_execTable[stageName]();
			return;
		}
		if (input.IsTriggered("back"))
		{
			SoundManager::GetInstance().StopSound("StageSelectBGM", true);
			_manager.ChangeSceneWithFade(std::make_shared<SceneTitle>(_manager), FadeState::NormalFadeIn);
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

void SceneStageSelect::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 背景
	if (_frame < kUIControllInterval && _frame > 0)
	{
		_bgOffsetX += 10;
	}
	if (_frame > -kUIControllInterval && _frame < 0)
	{
		_bgOffsetX -= 10;
	}
	DrawExtendGraph(-screenW + _bgOffsetX, 0, 0 + _bgOffsetX, screenH, _bgHandle, false);
	DrawExtendGraph(0 + _bgOffsetX, 0, screenW + _bgOffsetX, screenH, _bgHandle, false);
	DrawExtendGraph(screenW + _bgOffsetX, 0, screenW * 2 + _bgOffsetX, screenH, _bgHandle, false);

	// ステージUIの描画
	float progress = abs(static_cast<float>(_frame) / kUIControllInterval);
	int posX = _frame * kUIMoveScale;
	int sidePosX = kUIControllInterval * kUIMoveScale;
	SetDrawBright(128, 128, 128);	// 移動中のUIは暗く表示
	if (_isUIMoveRight)		// UIが右に移動するとき
	{
		// 最初のステージ以外を選択中
		const bool isSelectExceptFirst = (_selectIndex > 0);
		// 最後のステージ以外を選択中
		const bool isSelectExceptLast = (_selectIndex < _stageList.size() - 2);
		// 最後のステージの一つ手前を選択中
		const bool isSelectPenultimate = (_selectIndex == _stageList.size() - 3);

		if (isSelectExceptFirst)
		{
			DrawRotaGraph(screenW / 2 - sidePosX, screenH / 2, progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向の反対から現れるUI
		}
		if (isSelectExceptLast)
		{
			if (isSelectPenultimate)
			{
				if (_manager.GetSaveData().isReleasedSecretStage)
				{
					DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, kUIDrawScaleHalf - progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向で消えるUI
				}
			}
			else
			{
				DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, kUIDrawScaleHalf - progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向で消えるUI
			}
		}
		DrawRotaGraph(screenW / 2 - sidePosX + posX, screenH / 2, kUIDrawScaleHalf + progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向の反対から真ん中に向かうUI
	}
	else	// 左に移動するとき
	{
		// 最初のステージ以外を選択中
		const bool isSelectExceptFirst = (_selectIndex > 1);
		// 最後のステージ以外を選択中
		const bool isSelectExceptLast = (_selectIndex < _stageList.size() - 1);
		// 最後のステージの一つ手前を選択中
		const bool isSelectPenultimate = (_selectIndex == _stageList.size() - 2);

		if (isSelectExceptFirst)
		{
			DrawRotaGraph(screenW / 2 - sidePosX, screenH / 2, kUIDrawScaleHalf - progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向で消えるUI
		}
		if (isSelectExceptLast)
		{
			if (isSelectPenultimate)	// 隠しステージの前のステージを選択中の時
			{							// 隠しステージが解放されている場合のみUIを描画
				if (_manager.GetSaveData().isReleasedSecretStage)
				{
					DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向の反対から現れるUI
				}
			}
			else	// 隠しステージの前のステージ以外を選択中の時は普通に描画
			{
				DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向の反対から現れるUI
			}
		}
		DrawRotaGraph(screenW / 2 + sidePosX + posX, screenH / 2, kUIDrawScaleHalf + progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 進行方向の反対から真ん中に向かうUI
	}
	DrawRotaGraph(screenW / 2 + posX, screenH / 2, kUIDrawScale - progress * kUIDrawScaleHalf, 0.0, _emptyStageUIHandle, true);	// 真ん中から進行方向に向かうUI
	SetDrawBright(255, 255, 255);	// 明るさを元に戻す

	// UI移動中は描画しない
	if (_frame == kUIControllInterval || _frame == -kUIControllInterval)
	{
		//// ステージ名の描画
		//if (_selectIndex > 0 && _selectIndex < _stageList.size() - 1)		// 最初のステージと最後のステージ以外を選択中の時は選択中、左、右のステージ名を表示
		//{
		//	//DrawFormatString(screenW / 2 - 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
		//	//DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		//
		//	const bool isSelectPenultimate = (_selectIndex == static_cast<int>(SelectableStages::Num) - 3);	// 隠しステージの一つ手前を選択中かどうか
		//	if (isSelectPenultimate)	// 隠しステージの一つ手前を選択中の時
		//	{	// 隠しステージが解放されている場合のみ右のステージ名を表示
		//		if (_manager.GetSaveData().isReleasedSecretStage)
		//		{
		//			//DrawFormatString(screenW / 2 + 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
		//		}
		//	}
		//	else
		//	{
		//		//DrawFormatString(screenW / 2 + 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
		//	}
		//}
		//else if (_selectIndex == 0)	// 最初のステージを選択中のときは選択中と右のステージ名を表示
		//{
		//	//DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		//	//DrawFormatString(screenW / 2 + 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
		//}
		//else if (_selectIndex == _stageList.size() - 1)	// 最後のステージを選択中のときは選択中と左のステージ名を表示
		//{
		//	//DrawFormatString(screenW / 2 - 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
		//	//DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		//}

		// ステージアイコンの描画
		// 選択中のステージアイコン
		if (_selectIndex == _manager.GetSaveData().clearedStage)
		{
			// 未クリアのステージアイコン
			DrawRotaGraph(screenW / 2, screenH / 2, kUIDrawScale, 0.0, _stageShadowUIHandles[_selectIndex], true);
		}
		else
		{
			// 通常のステージアイコン
			DrawRotaGraph(screenW / 2, screenH / 2, kUIDrawScale, 0.0, _stageUIHandles[_selectIndex], true);
		}
		// 左右のステージアイコンは暗く表示
		SetDrawBright(128, 128, 128);
		// 左のステージアイコン
		if (_selectIndex != 0)	// 一番左以外を選択中
		{
			DrawRotaGraph(screenW / 2 - kUIControllInterval * kUIMoveScale, screenH / 2, kUIDrawScaleHalf, 0.0, _stageUIHandles[_selectIndex - 1], true);
		}
		// 右のステージアイコン
		if (_manager.GetSaveData().isReleasedSecretStage)	// 隠しステージを解放している場合
		{
			if (_selectIndex != static_cast<int>(SelectableStages::SecretStage) - 1)	// 一番右以外を選択中
			{
				// クリアしていないステージアイコンは未クリア用を使う
				if (_selectIndex + 1 >= _manager.GetSaveData().clearedStage)
				{
					DrawRotaGraph(screenW / 2 + kUIControllInterval * kUIMoveScale, screenH / 2, kUIDrawScaleHalf, 0.0, _stageShadowUIHandles[_selectIndex + 1], true);
				}
				else
				{
					DrawRotaGraph(screenW / 2 + kUIControllInterval * kUIMoveScale, screenH / 2, kUIDrawScaleHalf, 0.0, _stageUIHandles[_selectIndex + 1], true);
				}
			}
		}
		else	// 隠しステージを解放していない場合
		{
			if (_selectIndex != static_cast<int>(SelectableStages::Stage3) - 1)	// 一番右以外を選択中
			{
				// クリアしていないステージアイコンは未クリア用を使う
				if (_selectIndex + 1 >= _manager.GetSaveData().clearedStage)
				{
					DrawRotaGraph(screenW / 2 + kUIControllInterval * kUIMoveScale, screenH / 2, kUIDrawScaleHalf, 0.0, _stageShadowUIHandles[_selectIndex + 1], true);
				}
				else
				{
					DrawRotaGraph(screenW / 2 + kUIControllInterval * kUIMoveScale, screenH / 2, kUIDrawScaleHalf, 0.0, _stageUIHandles[_selectIndex + 1], true);
				}
			}
		}

		// 明るさを元に戻す
		SetDrawBright(255, 255, 255);

		// ステージ名の描画
		DrawRotaGraph(screenW / 2, screenH / 2 - 250, 0.7, 0.0, _stageNameHandles[_selectIndex], true);
		// ハイスコアの描画
		DrawRotaGraph(screenW / 2 - 100, screenH / 2 + 250, 0.5, 0.0, _highScoreTextHandle, true);
		IntGraphDrawer temp;
		temp.Draw(screenW / 2+20, screenH / 2 + 220,0.5f,
			_numberGraphHandle, _manager.GetSaveData().highScores[_selectIndex + 1]);
		//DrawFormatString(screenW / 2, 300, 0xffffff, "High Score : %d", _manager.GetSaveData().highScores[_selectIndex + 1]);

		// 隠しステージが解放されていない場合、隠しステージの前のステージを選択中の時に青い四角を描画
		if (!_manager.GetSaveData().isReleasedSecretStage &&
			_selectIndex == static_cast<int>(SelectableStages::Num) - 3)	// 隠しステージの一つ手前 = Num - 3
		{
			DrawBox(screenW / 2 + 250 - 25,
				screenH / 2 - 200,
				screenW / 2 + 250 + 35,
				screenH / 2 + 200,
				0x0000ff, true);
		}
		// 選択できないステージを表す赤い四角を描画
		if (_selectIndex == _manager.GetSaveData().clearedStage)
		{
			DrawBox(screenW / 2 + 250 - 25,
				screenH / 2 - 200,
				screenW / 2 + 250 + 25,
				screenH / 2 + 200,
				0xff0000, true);
		}
	}


#ifdef _DEBUG
	DrawString(0, 0, "SceneStageSelect",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "selectIndex:%d",_selectIndex);
	DrawFormatString(0, 100, 0xffffff, "frame:%d", _frame);
#endif
}