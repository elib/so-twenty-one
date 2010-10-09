#include "FlashingBomb.h"

void FlashingBomb::Initialize()
{
	GameObject::Initialize();

	//width of each frame is 32, and we want 5 frames per second.
	InitializeAnimation(32, 5);
}