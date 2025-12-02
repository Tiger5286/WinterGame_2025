#pragma once
#include "SceneBase.h"

class ScenePause :
    public SceneBase
{
public:
    ScenePause(SceneManager& manager);
    ~ScenePause();

	void Init() override;
    void Update(Input& input) override;
    void Draw() override;
};