#pragma once
class IntGraphDrawer
{
public:
	IntGraphDrawer();
	~IntGraphDrawer();

	void Draw(int value);
private:
	int CountDigits(int value);

private:
	int _handle;
};

