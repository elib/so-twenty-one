#pragma once

#include "GameObject.h"

class SpawnPoint : public GameObject
{
public:
	SpawnPoint(double x, double y) : GameObject("Resources/spawn.png", x, y) {}

};