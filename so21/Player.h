#pragma once
#include "gameobject.h"

class Player :
	public GameObject
{
public:
	Player(const char* filename, double x, double y) : GameObject(filename, x, y) {}
};
