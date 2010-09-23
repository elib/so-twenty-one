#include "Keyboard.h"

Keyboard::Keyboard(void)
{
}

Keyboard::~Keyboard(void)
{
}


void Keyboard::Initialize()
{
	ZeroKeys(keys_down);
	ZeroKeys(keys_just_down);
}

void Keyboard::ZeroKeys(bool * keys)
{
	//use some array magic to zero the keys quickly
	memset((void*)keys, 0, NUM_OF_KEYS * sizeof(bool));
}

void Keyboard::ConsumeKeyDown(ALLEGRO_KEYBOARD_EVENT event)
{
	keys_down[event.keycode] = true;
	keys_just_down[event.keycode] = true;
}

void Keyboard::ConsumeKeyUp(ALLEGRO_KEYBOARD_EVENT event)
{
	keys_down[event.keycode] = false;
}

//update at the END of the frame
void Keyboard::Update()
{
	ZeroKeys(keys_just_down);
}