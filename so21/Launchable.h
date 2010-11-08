#pragma once
#include "GameObject.h"

class Launchable : public GameObject
{
public:
	Launchable(const char* filename, double x, double y) : GameObject(filename, x, y) {}
	virtual ~Launchable(void) {}

	virtual void Launch(double X, double Y) = 0;
};