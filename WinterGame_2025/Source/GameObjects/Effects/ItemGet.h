#pragma once
#include "Effect.h"
class ItemGet :
    public Effect
{
public:
    ItemGet(int handle,Vector2 pos);
private:
    int _handle = -1;
};

