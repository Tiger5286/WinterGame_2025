#pragma once

class Input;
class SceneManager;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	SceneBase(SceneManager& manager);
	virtual ~SceneBase();

	virtual void Init() = 0;
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
protected:
	SceneManager& _manager;
};