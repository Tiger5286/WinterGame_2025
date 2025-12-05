#pragma once
#include "Effect.h"
class ItemGet :
    public Effect
{
public:
    ItemGet(int handle,Vector2 pos);

    void Update();
    void Draw(Vector2 offset);
private:
    int _handle = -1;
};

