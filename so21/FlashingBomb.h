#pragma once

#include "GameObject.h"

class FlashingBomb : public GameObject
{
public:
	FlashingBomb(double X, double Y) : GameObject("Resources/flashing-bomb.png", X, Y) {}
	virtual void Initialize();
};
