#pragma once
class IntGraphDrawer
{
public:
	IntGraphDrawer();
	~IntGraphDrawer();

	void Draw(int x,int y, float scale, int handle, int value);
private:
	int CountDigits(int value);
	int PickOutDigit(int value, int digit);
};

