#include "Laser.h"
#include "World.h"

void Laser::Initialize()
{
	GameObject::Initialize();

	//width of each frame is 32, and we want 5 frames per second.
	InitializeAnimation(32, 8);
	size = Vec2(32, 32);
	//bounding_box.x = 

	_speed = 100;
}


void Laser::Launch(double X, double Y)
{
	position = Vec2(X, Y);
	velocity = Vec2(-_speed,0);
}