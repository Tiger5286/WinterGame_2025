#include "SceneBase.h"
#include "Fade.h"	// 子クラスで使用するためインクルードしておく

SceneBase::SceneBase(SceneManager& manager):
	_manager(manager)
{
}

SceneBase::~SceneBase()
{
}