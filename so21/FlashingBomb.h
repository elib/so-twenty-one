#pragma once

#include "Launchable.h"

class FlashingBomb : public Launchable
{
public:
	FlashingBomb(double X, double Y) : Launchable("Resources/flashing-bomb.png", X, Y) {}
	virtual void Initialize();
	virtual void Launch(double X, double Y);

private:
	double _speed;
};
