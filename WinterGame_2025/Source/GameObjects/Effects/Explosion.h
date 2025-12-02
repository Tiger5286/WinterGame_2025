#pragma once
#include "Effect.h"
class Explosion :
    public Effect
{
public:
    Explosion(int handle,Vector2 pos);

    void Update();
    void Draw(Vector2 offset);
private:
    int _handle = -1;
};

