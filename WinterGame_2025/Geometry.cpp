#include "Geometry.h"
#include <cmath>

Vector2::Vector2():
	x(0.0f),
	y(0.0f)
{
}

Vector2::Vector2(float x, float y):
	x(x),
	y(y)
{
}

Vector2::~Vector2()
{
}

float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

void Vector2::Normalize()
{
	float len = Length();
	if (len == 0.0f)
	{
		x = 0.0f;
		y = 0.0f;
	}
	else
	{
		x /= len;
		y /= len;
	}
}

Vector2 Vector2::Normalized() const
{
	float len = Length();
	if (len == 0.0f)
	{
		return { 0.0f,0.0f };
	}
	else
	{
		return { x / len,y / len };
	}
}

Vector2 Vector2::Vlerp(Vector2 start, Vector2 end, float t)
{
	Vector2 ans;
	ans.x = std::lerp(start.x, end.x, t);
	ans.y = std::lerp(start.y, end.y, t);
	return ans;
}

Vector2 Vector2::operator+(const Vector2 vec) const
{
	return { x + vec.x,y + vec.y };
}

void Vector2::operator+=(const Vector2 vec)
{
	x += vec.x;
	y += vec.y;
}

Vector2 Vector2::operator-(const Vector2 vec) const
{
	return { x - vec.x,y - vec.y };
}

void Vector2::operator-=(const Vector2 vec)
{
	x -= vec.x;
	y -= vec.y;
}

Vector2 Vector2::operator*(const float scale) const
{
	return { x * scale,y * scale };
}

void Vector2::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 Vector2::operator/(const float scale) const
{
	return { x / scale,y / scale };
}

void Vector2::operator/=(const float scale)
{
	x /= scale;
	y /= scale;
}

Vector2 Vector2::operator-() const
{
	return { -x,-y };
}
