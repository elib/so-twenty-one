#pragma once

#include <allegro5/allegro.h>

class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);

	void Initialize();
	void ConsumeKeyDown(ALLEGRO_KEYBOARD_EVENT event);
	void ConsumeKeyUp(ALLEGRO_KEYBOARD_EVENT event);

	unsigned bool keys_down[256];
	unsigned bool keys_just_down[256];
};
