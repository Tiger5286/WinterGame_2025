#pragma once
#include "Effect.h"

enum class ExplosionType
{
    Normal,
    Small,
    Big
};

class Explosion :
    public Effect
{
public:
    Explosion(int handle,Vector2 pos,ExplosionType type = ExplosionType::Normal);
private:
    int _handle = -1;
};

