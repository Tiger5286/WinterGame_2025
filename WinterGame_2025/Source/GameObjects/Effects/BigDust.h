#pragma once
#include "Effect.h"
class BigDust :
    public Effect
{
public:
    BigDust(int handle,Vector2 pos);

private:
    int _handle = -1;
};

