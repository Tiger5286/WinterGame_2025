#pragma once
#include <memory>

class Input;
class SceneBase;

/// <summary>
/// 内部にシーンを保持し、切り替えなどを行うクラス
/// </summary>
class SceneManager
{
private:
	std::shared_ptr<SceneBase> _scene;
public:
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	void Init();
	void Update(Input input);
	void Draw();
};

