#pragma once
#include "Geometry.h"
class Animation
{
public:
	Animation();
	~Animation();

	void Init(int drawHandle,int animIndex, Vector2 frameSize,int maxAnimNum,int oneAnimFrame,float scale);
	void Update();
	void Draw(Vector2 pos,bool isTurn);

	bool operator!=(const Animation& other) const;
private:
	int _drawHandle;	// 画像ハンドル
	int _animIndex;		// アニメーションの種類番号
	Vector2 _frameSize;	// 画像の1コマサイズ
	int _maxAnimNum;	// アニメーションの最大コマ数
	int _nowAnimNum;	// 現在のアニメーション番号
	int _oneAnimFrame;	// 1コマあたりのフレーム数
	int _frameCount;	// アニメーション制御用
	float _scale;		// 拡大率
	float _rotate;		// 回転角
};