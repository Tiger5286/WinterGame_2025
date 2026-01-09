#pragma once
class IntGraphDrawer
{
public:
	IntGraphDrawer();
	~IntGraphDrawer();

	static void Draw(int x,int y, float scale, int handle, int value);
private:
	/// <summary>
	/// 桁数を数える
	/// </summary>
	/// <param name="value">桁数を数えたい数値</param>
	/// <returns>桁数</returns>
	static int CountDigits(int value);
	/// <summary>
	/// 指定された桁の数字を抽出します。
	/// </summary>
	/// <param name="value">抽出対象の整数値。</param>
	/// <param name="digit">抽出したい桁の位置（0から始まるインデックス、右端が0）。</param>
	/// <returns>指定された桁の数字（0～9の整数）。</returns>
	static int PickOutDigit(int value, int digit);
};

