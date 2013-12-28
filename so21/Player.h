#pragma once
#include "gameobject.h"
#include "VisualMessage.h"

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
	virtual bool Collide(GameObject *otherobj);
	virtual void OnCollide(GameObject *otherobj, b2Contact *contact);
protected:
	Vec2 _starting_location;
	bool _notifiedPlayerLeftSpawn;
	VisualMessage _playermessage;
	virtual b2BodyDef GetBodyDef();
};