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
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	void Init();
	void Update(Input input);
	void Draw();
private:
	std::shared_ptr<SceneBase> _pScene;
	std::shared_ptr<Fade> _pFade;
};