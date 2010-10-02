#pragma once
#include "gameobject.h"

class Player :
	public GameObject
{
public:
	Player(double x, double y) : GameObject("Resources/probe-sprite.png", x, y)
	{
		_starting_location = position;
		_notifiedPlayerLeftSpawn = false;
	}

	virtual void Initialize();
	virtual void Update(double delta_time);
	virtual void Collide(GameObject *otherobj);
protected:
	Vec2 _starting_location;
	bool _notifiedPlayerLeftSpawn;
};