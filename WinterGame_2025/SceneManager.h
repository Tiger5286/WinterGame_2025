#pragma once
#include <memory>
#include "Fade.h"

class Input;
class SceneBase;
class Fade;

/// <summary>
/// 内部にシーンを保持し、切り替えなどを行うクラス
/// </summary>
class SceneManager
{
public:
	SceneManager();

	void Init();
	void Update(Input input);
	void Draw();

	/// <summary>
	/// シーンを変更する
	/// </summary>
	/// <param name="scene">次に再生するシーン</param>
	/// <param name="fadeType">次に再生するシーン開始時に行うフェードの種類</param>
	void ChangeScene(std::shared_ptr<SceneBase> scene,FadeState fadeType = FadeState::NormalFadeIn);

	/// <summary>
	/// フェードアウトを行い、シーンを変更する
	/// </summary>
	/// <param name="scene">次に再生するシーン</param>
	/// <param name="nextFadeType">次に再生するシーン開始時に行うフェードインの種類</param>
	/// <param name="fadeType">シーン終了時に行うフェードアウトの種類</param>
	void ChangeSceneWithFade(std::shared_ptr<SceneBase> scene, FadeState nextFadeType = FadeState::NormalFadeIn, FadeState fadeType = FadeState::NormalFadeOut);
	
	void SetFadeCirclePos(const Vector2& pos) { _pFade->SetCirclePos(pos); }

private:
	std::shared_ptr<SceneBase> _pScene;
	std::shared_ptr<Fade> _pFade;
	std::shared_ptr<SceneBase> _pNextScene;
	FadeState _nextFadeType;
};