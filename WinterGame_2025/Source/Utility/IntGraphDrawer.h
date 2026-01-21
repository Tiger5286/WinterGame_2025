#pragma once
class IntGraphDrawer
{
public:
	/// <summary>
	/// 整数を画像に変換して描画する
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="scale">描画倍率</param>
	/// <param name="handle">画像ハンドル</param>
	/// <param name="value">数値</param>
	/// <param name="digits">何桁表示するか</param>
	static void Draw(int x,int y, float scale, int handle, int value,int drawDigits = 5);
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

