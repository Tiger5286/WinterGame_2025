#pragma once
#include "SceneBase.h"
class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	~SceneTitle();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
};

