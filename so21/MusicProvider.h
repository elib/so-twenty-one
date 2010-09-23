#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_windows.h>

#include "Bass.h"

class MusicProvider
{
public:
	MusicProvider(void);
	~MusicProvider(void);

	void Initialize(ALLEGRO_DISPLAY *display);
protected:
	static const char* _music_file;
	HSTREAM _stream;
};
