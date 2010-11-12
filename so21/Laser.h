#pragma once

#include "Launchable.h"

class Laser : public Launchable
{
public:
	Laser(double X, double Y) : Launchable("Resources/laser_yellow.png", X, Y) {}
	virtual void Initialize();
	virtual void Launch(double X, double Y);

private:
	double _speed;
};
