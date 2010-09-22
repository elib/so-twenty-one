#pragma once
#include "gameobject.h"

class Player :
	public GameObject
{
public:
	Player(double x, double y) : GameObject("Resources/probe-sprite.png", x, y) {}
};
