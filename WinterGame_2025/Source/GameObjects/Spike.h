#pragma once
#include "Gimmick.h"

class Player;

class Spike :
    public Gimmick
{
public:
	Spike(const Vector2& chipPos, std::shared_ptr<Player> pPlayer, int handle);
    ~Spike();

    void Init() override;
	void Update(Map& map) override;
	void Update() override;
	void Draw(Vector2 offset) override;

	GimmickType GetType() const override { return GimmickType::Spike; }
private:
	int _handle;
	int _frame = 0;
};

