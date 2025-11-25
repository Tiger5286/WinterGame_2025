#pragma once
#include <memory>

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
	void ChangeScene(std::shared_ptr<SceneBase> scene);

	void ChangeSceneWithFadeOut(std::shared_ptr<SceneBase> scene);

private:
	std::shared_ptr<SceneBase> _pScene;
	std::shared_ptr<Fade> _pFade;
	std::shared_ptr<SceneBase> _pNextScene;
};