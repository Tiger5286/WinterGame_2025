#pragma once
class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	~Vector2();

	/// <summary>
	/// 自身のベクトルの長さを返す
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float Length() const;

	/// <summary>
	/// 自身のベクトルを正規化する
	/// </summary>
	void Normalize();

	/// <summary>
	/// 自身のベクトルを正規化したベクトルを返す
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector2 Normalized() const;

	// 演算子オーバーロード
	// 足し算
	Vector2 operator+(const Vector2 vec) const;
	void operator+=(const Vector2 vec);
	// 引き算
	Vector2 operator-(const Vector2 vec) const;
	void operator-=(const Vector2 vec);
	// 掛け算(スケーリング)
	Vector2 operator*(const float scale) const;
	void operator*=(const float scale);
	// 割り算
	Vector2 operator/(const float scale) const;
	void operator/=(const float scale);
	// 符号を逆転
	Vector2 operator-() const;
};

