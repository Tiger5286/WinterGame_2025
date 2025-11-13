#pragma once
#include "SceneBase.h"
class SceneClear : public SceneBase
{
public:
	SceneClear(SceneManager& manager);
	~SceneClear();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
};

