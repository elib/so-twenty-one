#include "Fader.h"

#define FADE_SPEED 1


void Fader::Pulse()
{
	alpha = 1.0;
}

void Fader::Initialize()
{
	GameObject::Initialize();
	bounding_box_debug = false;
	alpha = 0;
	strcpy_s(prettyName, "Fader");
}

void Fader::Update(double delta_time)
{
	if(alpha > 0)
	{
		alpha -= delta_time * FADE_SPEED;
	}

	GameObject::Update(delta_time);
}