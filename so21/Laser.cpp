#include "Laser.h"
#include "World.h"

void Laser::Initialize()
{
	GameObject::Initialize();

	//width of each frame is 32, and we want 5 frames per second.
	InitializeAnimation(32, 8);
	size = Vec2(32, 32);

	bounding_box.x = 2;
	bounding_box.y = 15;
	bounding_box.width = 27;
	bounding_box.height = 4;

	strcpy_s(prettyName, "Laser");

	_speed = 100;
	_is_static = false;
}


void Laser::Launch(double X, double Y)
{
	position = Vec2(X, Y);
	velocity = Vec2(-_speed,0);
}