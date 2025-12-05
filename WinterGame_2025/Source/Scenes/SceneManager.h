#pragma once
#include <memory>
#include <list>
#include "../Systems/Fade.h"
#include "../Game.h"

class Input;
class SceneBase;
class Fade;
class DataManager;
class DebugScene;

/// <summary>
/// 内部にシーンを保持し、切り替えなどを行うクラス
/// </summary>
class SceneManager
{
	friend DebugScene;
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Update(Input input);
	void Draw();

	/// <summary>
	/// 積んでいるシーンを全てクリアし、新たなシーンを再生する
	/// </summary>
	/// <param name="pScene">再生するシーン</param>
	void ResetScene(std::shared_ptr<SceneBase> pScene);

	/// <summary>
	/// シーンを積む
	/// </summary>
	/// <param name="pScene">シーン</param>
	void PushScene(std::shared_ptr<SceneBase> pScene);

	/// <summary>
	/// 一番上に積んでいるシーンを消す
	/// </summary>
	void PopScene();

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
	
	/// <summary>
	/// 丸フェードの位置を設定する
	/// </summary>
	/// <param name="pos">丸フェードの位置</param>
	void SetFadeCirclePos(const Vector2& pos) { _pFade->SetCirclePos(pos); }

	/// <summary>
	/// 指定されたフレームの間、シーンの更新を停止する
	/// </summary>
	/// <param name="frame">停止するフレーム数</param>
	void Stop(int frame) { _stopFrame = frame; }

	/// <summary>
	/// クリア済みの最後のステージを取得する
	/// </summary>
	/// <returns>クリア済みステージ</returns>
	Stages GetClearedStage() const { return _clearedStage; }

	/// <summary>
	/// クリアしたステージが未クリアなら、クリア済みとして登録する
	/// </summary>
	/// <param name="clearedStage">クリアしたステージ</param>
	void CheckClearedStage(Stages clearedStage);

private:
	std::list<std::shared_ptr<SceneBase>> _pScenes;
	std::shared_ptr<Fade> _pFade;
	std::shared_ptr<SceneBase> _pNextScene;
	FadeState _nextFadeType;
	int _stopFrame;
	Stages _clearedStage = Stages::None;

	std::shared_ptr<DataManager> _pDataManager;
};