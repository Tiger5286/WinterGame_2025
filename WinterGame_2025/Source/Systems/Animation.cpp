#include "Animation.h"
#include "Dxlib.h"

Animation::Animation():
	_isRepeat(true),
	_drawHandle(-1),
	_animIndex(0),
	_frameSize({ 0.0f,0.0f }),
	_maxAnimNum(0),
	_nowAnimNum(0),
	_oneAnimFrame(0),
	_frameCount(0),
	_scale(1.0f),
	_rotate(0.0f)
{
}

Animation::~Animation()
{
}

void Animation::Init(int drawHandle, int animIndex, Vector2 frameSize, int maxAnimNum, int oneAnimFrame, float scale,bool isRepeat)
{
	_isRepeat = isRepeat;
	_drawHandle = drawHandle;
	_animIndex = animIndex;
	_frameSize = frameSize;
	_maxAnimNum = maxAnimNum;
	_oneAnimFrame = oneAnimFrame;
	_scale = scale;
	_nowAnimNum = 0;
	_frameCount = 0;
}

void Animation::Init(int drawHandle, int animIndexY, int animIndexX, Vector2 frameSize, float scale, bool isRepeat)
{
	_isRepeat = isRepeat;
	_drawHandle = drawHandle;
	_animIndex = animIndexY;
	_frameSize = frameSize;
	_maxAnimNum = animIndexX + 1;
	_oneAnimFrame = 0;
	_scale = scale;
	_nowAnimNum = animIndexX;
	_frameCount = 0;
}

void Animation::Update()
{
	if (_oneAnimFrame == 0) return;	// 1コマあたりのフレーム数が0なら処理を抜ける

	_frameCount++;	// フレームカウントを進める
	
	if (_frameCount >= _oneAnimFrame)	// 1コマ分のフレームが経過したら
	{
		_frameCount = 0;
		_nowAnimNum++;	// アニメーション番号を進める
		if (_nowAnimNum >= _maxAnimNum)
		{	// 最大コマ数を超えたら最初に戻す
			if (_isRepeat)
			{	// 繰り返し再生するなら最初のコマに戻す
				_nowAnimNum = 0;
			}
			
		}
	}
}

void Animation::Draw(Vector2 pos, bool isTurn)
{
	// 現在のコマ数が最大コマ数なら描画しない
	if (_nowAnimNum != _maxAnimNum)
	{
		DrawRectRotaGraph(pos.x + _drawOffset.x, pos.y + _drawOffset.y,
			_frameSize.x * _nowAnimNum, _animIndex * _frameSize.y,
			_frameSize.x, _frameSize.y,
			_scale, _rotate, _drawHandle, true, isTurn);
	}
}

void Animation::Draw(int drawHandle, Vector2 pos, bool isTurn)
{
	if (_nowAnimNum != _maxAnimNum)
	{
		DrawRectRotaGraph(pos.x, pos.y,
			_frameSize.x * _nowAnimNum, _animIndex * _frameSize.y,
			_frameSize.x, _frameSize.y,
			_scale, _rotate, drawHandle, true, isTurn);
	}
}

bool Animation::GetIsEnd()
{
	if (_isRepeat) return false;	// 繰り返し再生するなら終了しない

	int totalFrames = _oneAnimFrame * _maxAnimNum;	// アニメーション全体のフレーム数
	int currentFrame = _nowAnimNum * _oneAnimFrame + _frameCount;	// 現在のフレーム数
	if (currentFrame >= totalFrames) return true;

	return false;
}

bool Animation::operator!=(const Animation& other) const
{
	if (_drawHandle != other._drawHandle) return true;
	if (_animIndex != other._animIndex) return true;
	if (_rotate != other._rotate) return true;
	if (_oneAnimFrame != other._oneAnimFrame) return true;
	return false;
}
