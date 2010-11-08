#include "FlashingBomb.h"
#include "World.h"

void FlashingBomb::Initialize()
{
	GameObject::Initialize();

	//width of each frame is 32, and we want 5 frames per second.
	InitializeAnimation(32, 5);
	size = Vec2(32, 32);
	//bounding_box.x = 

	_speed = 80;
}


void FlashingBomb::Launch(double X, double Y)
{
	position = Vec2(X, Y);
	velocity = _speed * ((World::theWorld->PlayerPosition() - position).Normalise());
}