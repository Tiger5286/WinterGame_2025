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
#include "../Systems/EffectManager.h"
#include <cassert>

namespace
{
	// ステージアイコン関連定数
	constexpr int kUIControllInterval = 10;	// ステージアイコンが移動する時間(操作の効かない時間)
	constexpr int kUIMoveScale = 50;
	constexpr float kUIDrawScale = 0.7f;
	constexpr float kUIDrawScaleHalf = kUIDrawScale / 2;
	// テキスト画像関連
	// ステージ名
	constexpr int kStageNameY = GlobalConstants::kScreenHeight / 2 - 350;
	constexpr float kStageNameScale = 1.0f;
	// ハイスコア
	constexpr int kHighScoreTextX = GlobalConstants::kScreenWidth / 2 - 100;
	constexpr int kHighScoreTextY = GlobalConstants::kScreenHeight / 2 + 325;
	constexpr float kHighScoreTextScale = 0.6f;
	// ハイスコア(数字部分)
	constexpr int kHighScoreNumberX = GlobalConstants::kScreenWidth / 2 + 40;
	constexpr int kHighScoreNumberY = GlobalConstants::kScreenHeight / 2 + 285;
	constexpr float kHighScoreNumberScale = 0.6f;

	// 背景
	// ステージアイコン選択時にどれだけ背景が動くか
	constexpr int kDrawBgGap = 10;

	// 隠しステージ関連
	// 何回右入力したら解放されるか
	constexpr int kRequiredNumberOfPress = 10;
	constexpr int kReleaseFrame = 100;
	constexpr int kExplosionInterval = 10;
	constexpr int kExplosionPosAmplitude = 400;

	// レーザー
	constexpr int kLaserGraphWidth = 16;
	constexpr int kLaserGraphHeight = 120;
	constexpr int kLaserAnimNum = 4;
	constexpr int kLaserOneAnimFrame = 6;
	constexpr int kLaserPosX = GlobalConstants::kScreenWidth / 2 + 325;
	constexpr float kLaserScale = 9.0f;
}

SceneStageSelect::SceneStageSelect(SceneManager& manager, Stages playedStage):
	SceneBase(manager),
	_uiControllFrame(kUIControllInterval)
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
	// レーザーの画像ロード
	_longLaserHandle = LoadGraph("data/UI/LongLaser.png");

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
		_uiControllFrame = -kUIControllInterval;
	}

	// エフェクトマネージャーを生成
	_pEffectManager = std::make_shared<EffectManager>();

	// bgm再生
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
}

void SceneStageSelect::Init()
{
}

void SceneStageSelect::Update(Input& input)
{
	// UI移動演出用フレームカウントの更新
	if (_uiControllFrame < kUIControllInterval && _isUIMoveRight)
	{	// 右移動中
		_uiControllFrame++;
	}
	if (_uiControllFrame > -kUIControllInterval && !_isUIMoveRight)
	{	// 左移動中
		_uiControllFrame--;
	}

	// UI移動演出中は操作が効かないようにする
	if (_uiControllFrame == kUIControllInterval || _uiControllFrame == -kUIControllInterval)
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
						_uiControllFrame = 0;
						SoundManager::GetInstance().PlaySoundGame("Select");
					}
					else	// 隠しステージが解放されていない場合、右入力カウントを増やす
					{
						_inputRightCount++;
						if (_inputRightCount >= kRequiredNumberOfPress)	// n回右入力したら隠しステージ解放
						{
							_isNowReleasedSecretStage = true;
						}
						SoundManager::GetInstance().PlaySoundGame("Beep");
					}
				}
				else // 通常のステージ移動
				{
					_selectIndex++;
					_isUIMoveRight = false;
					_uiControllFrame = 0;
					SoundManager::GetInstance().PlaySoundGame("Select");
				}
			}
			else
			{
				SoundManager::GetInstance().PlaySoundGame("Beep");
			}
		}
		if (input.IsTriggered("left"))
		{
			if (_selectIndex > 0)
			{
				_selectIndex--;
				_isUIMoveRight = true;
				_uiControllFrame = 0;
				SoundManager::GetInstance().PlaySoundGame("Select");
			}
			else
			{
				SoundManager::GetInstance().PlaySoundGame("Beep");
			}
		}
		if (input.IsTriggered("decision"))
		{
			SoundManager::GetInstance().StopSound("StageSelectBGM", true);
			SoundManager::GetInstance().PlaySoundGame("Decision");
			auto& stageName = _stageList[_selectIndex];
			_execTable[stageName]();
			return;
		}
		if (input.IsTriggered("back"))
		{
			SoundManager::GetInstance().StopSound("StageSelectBGM", true);
			SoundManager::GetInstance().PlaySoundGame("Cancel");
			_manager.ChangeSceneWithFade(std::make_shared<SceneTitle>(_manager), FadeState::NormalFadeIn);
			return;
		}
	}

	// 隠しステージ解放の演出関連
	if (_isNowReleasedSecretStage)
	{
		_releasedFrameCount++;
		if (_releasedFrameCount % kExplosionInterval == 0)
		{
			SoundManager::GetInstance().PlaySoundGame("EnemyDeath");
		}
		if (_releasedFrameCount > kReleaseFrame)
		{
			_isNowReleasedSecretStage = false;
			_pEffectManager->Create(Vector2{ GlobalConstants::kScreenWidth / 2 + kUIControllInterval * kUIMoveScale, GlobalConstants::kScreenHeight / 2 }, EffectType::ExplosionHuge);
			_manager.ReleaseSecretStage();
			SoundManager::GetInstance().PlaySoundGame("ExplosionLong");
		}
	}

	_pEffectManager->Update();

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
	if (_uiControllFrame < kUIControllInterval && _uiControllFrame > 0)
	{
		_bgOffsetX += kDrawBgGap;
	}
	if (_uiControllFrame > -kUIControllInterval && _uiControllFrame < 0)
	{
		_bgOffsetX -= kDrawBgGap;
	}
	DrawExtendGraph(-screenW + _bgOffsetX, 0, 0 + _bgOffsetX, screenH, _bgHandle, false);
	DrawExtendGraph(0 + _bgOffsetX, 0, screenW + _bgOffsetX, screenH, _bgHandle, false);
	DrawExtendGraph(screenW + _bgOffsetX, 0, screenW * 2 + _bgOffsetX, screenH, _bgHandle, false);

	// 移動中のステージアイコン描画(移動中のみ描画)
	if (!(_uiControllFrame == kUIControllInterval || _uiControllFrame == -kUIControllInterval))
	{
		float progress = abs(static_cast<float>(_uiControllFrame) / kUIControllInterval);
		int posX = _uiControllFrame * kUIMoveScale;
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
	}

	// UI移動中は描画しない
	if (_uiControllFrame == kUIControllInterval || _uiControllFrame == -kUIControllInterval)
	{
		// 先に行けないことを表すレーザーの描画
		if (_selectIndex == _manager.GetSaveData().clearedStage &&
			_selectIndex < 3)
		{
			_laserAnimFrame++;

			int animIndex = (_laserAnimFrame / kLaserOneAnimFrame) % kLaserAnimNum;

			DrawRectRotaGraph(kLaserPosX, screenH / 2,
				animIndex * kLaserGraphWidth, 0,
				kLaserGraphWidth, kLaserGraphHeight,
				kLaserScale, 0.0, _longLaserHandle, true);
		}

		// ステージアイコンの描画
		// 選択中のステージアイコン
		auto iconHandle = _stageUIHandles[_selectIndex];
		if (_selectIndex == _manager.GetSaveData().clearedStage)
		{
			// 未クリアのステージアイコン
			iconHandle = _stageShadowUIHandles[_selectIndex];
		}
		_frame++;
		float sin = sinf(_frame * 0.05f) * 0.02f;
		DrawRotaGraph(screenW / 2, screenH / 2, kUIDrawScale + sin, 0.0, iconHandle, true);
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
		DrawRotaGraph(screenW / 2, kStageNameY, kStageNameScale, 0.0, _stageNameHandles[_selectIndex], true);
		// ハイスコアの描画
		DrawRotaGraph(kHighScoreTextX, kHighScoreTextY, kHighScoreTextScale, 0.0, _highScoreTextHandle, true);
		IntGraphDrawer::Draw(kHighScoreNumberX, kHighScoreNumberY, kHighScoreNumberScale,
			_numberGraphHandle, _manager.GetSaveData().highScores[_selectIndex + 1]);
	}

	// 隠しステージ解放時の演出描画
	if (_isNowReleasedSecretStage)
	{
		if (_releasedFrameCount % kExplosionInterval == 0)
		{
			// エフェクト生成位置
			Vector2 effectPos = { GlobalConstants::kScreenWidth / 2 + kUIControllInterval * kUIMoveScale, GlobalConstants::kScreenHeight / 2 };
			// -n~nのランダム数値
			effectPos.x += GetRand(kExplosionPosAmplitude) - kExplosionPosAmplitude / 2;
			effectPos.y += GetRand(kExplosionPosAmplitude) - kExplosionPosAmplitude / 2;
			// エフェクト生成
			_pEffectManager->Create(effectPos,EffectType::ExplosionBig);
		}
	}
	_pEffectManager->Draw(Vector2(0.0f,0.0f));

#ifdef _DEBUG
	DrawString(0, 0, "SceneStageSelect",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "selectIndex:%d",_selectIndex);
	DrawFormatString(0, 100, 0xffffff, "frame:%d", _uiControllFrame);
#endif
}