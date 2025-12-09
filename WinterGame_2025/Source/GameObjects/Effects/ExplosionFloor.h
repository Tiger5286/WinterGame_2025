#pragma once
#include "Effect.h"
class ExplosionFloor :
    public Effect
{
public:
    ExplosionFloor(int handle, Vector2 pos);
private:
    int _handle;
};

