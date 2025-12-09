#pragma once
#include "Effect.h"
class ExplosionUpward :
    public Effect
{
public:
    ExplosionUpward(int handle,Vector2 pos);
private:
    int _handle;
};

