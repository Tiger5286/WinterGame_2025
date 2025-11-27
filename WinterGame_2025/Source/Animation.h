#pragma once
#include "Utility/Geometry.h"
class Animation
{
public:
	Animation();
	~Animation();

	void Init(int drawHandle,int animIndex, Vector2 frameSize,int maxAnimNum,int oneAnimFrame,float scale,bool isRepeat = true);	// 普通のアニメーション初期化
	void Init(int drawHandle, int animIndexY, int animIndexX, Vector2 frameSize, float scale, bool isRepeat = true);	// 一枚絵アニメーション初期化
	void Update();
	void Draw(Vector2 pos,bool isTurn);
	void Draw(int drawHandle,Vector2 pos, bool isTurn);

	void SetRotate(float rotate) { _rotate = rotate; }
	void SetOffset(Vector2 offset) { _drawOffset = offset; }

	// アニメーションを最初から再生する
	void SetFirst() { _nowAnimNum = 0; _frameCount = 0; }
	// アニメーションを最後のコマにする
	void SetEnd() { _nowAnimNum = _maxAnimNum; }

	bool GetIsEnd();

	bool operator!=(const Animation& other) const;
private:
	bool _isRepeat;		// 繰り返し再生するかどうか
	int _drawHandle;	// 画像ハンドル
	int _animIndex;		// アニメーションの種類番号
	Vector2 _frameSize;	// 画像の1コマサイズ
	int _maxAnimNum;	// アニメーションの最大コマ数
	int _nowAnimNum;	// 現在のアニメーション番号
	int _oneAnimFrame;	// 1コマあたりのフレーム数
	int _frameCount;	// アニメーション制御用
	float _scale;		// 拡大率
	float _rotate;		// 回転角
	Vector2 _drawOffset; // 描画位置オフセット
};