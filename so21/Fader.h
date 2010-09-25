#pragma once
#include "gameobject.h"

class Fader : public GameObject
{
public:
	Fader(double x, double y) : GameObject("Resources/debug_circle.png", x, y) {}

	void Pulse();
	virtual void Initialize();
	virtual void Update(double delta_time);

protected:

};