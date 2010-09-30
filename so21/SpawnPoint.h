#pragma once

#include "GameObject.h"
#include <vector>
using namespace std;


class SpawnPoint : public GameObject
{
public:

	SpawnPoint(double x, double y) : GameObject("Resources/spawn.png", x, y) {}
	virtual void Initialize();
	virtual ~SpawnPoint();
	virtual void Update(double delta_time);
	virtual void Collide(GameObject *otherobj);

protected:
	vector<GameObject*> _sub_colliders;

};