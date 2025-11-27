#include "SceneBase.h"
#include "../Systems/Fade.h"	// 子クラスで使用するためインクルードしておく

SceneBase::SceneBase(SceneManager& manager):
	_manager(manager)
{
}

SceneBase::~SceneBase()
{
}