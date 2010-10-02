#pragma once

#include <allegro5/allegro.h>

class ScreenFade
{
public:
	ScreenFade(void);
	~ScreenFade(void);

	void Initialize();
	void StartFade(double start_time, double end_time);
	void Update(double current_time);
protected:
	ALLEGRO_COLOR _tint_color;
	double _end_time;
	double _start_time;

	int _wid, _hei;

	ALLEGRO_BITMAP *_bitmap;

	static const char* _bitmap_file;

};