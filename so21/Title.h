#pragma once

#include "GameObject.h"


class Title : public GameObject
{
public:
	Title(double X, double Y) : GameObject(X, Y) { _title1 = _title2 = NULL; }
	~Title();

	virtual void Initialize();
	virtual void Update(double delta_time);

private:
	GameObject* _title1;
	GameObject* _title2;
};

