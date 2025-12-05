#pragma once
#include "Effect.h"
class SmallDust :
    public Effect
{
public:
    SmallDust(int handle, Vector2 pos);
private:
    int _handle;
};