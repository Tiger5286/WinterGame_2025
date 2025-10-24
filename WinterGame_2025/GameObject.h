#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

protected:
	int _handle;
};

