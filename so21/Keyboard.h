#pragma once

#include <allegro5/allegro.h>

#define NUM_OF_KEYS		256

class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);

	void Initialize();
	void ConsumeKeyDown(ALLEGRO_KEYBOARD_EVENT event);
	void ConsumeKeyUp(ALLEGRO_KEYBOARD_EVENT event);

	bool keys_down[NUM_OF_KEYS];
	bool keys_just_down[NUM_OF_KEYS];

	//update at the END of the frame
	void Update();
protected:
	void ZeroKeys(bool * keys);
};