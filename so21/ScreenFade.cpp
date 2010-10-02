#include "ScreenFade.h"
#include "world.h"

const char* ScreenFade::_bitmap_file = "Resources/white.png";

ScreenFade::ScreenFade(void)
{
}

ScreenFade::~ScreenFade(void)
{
	al_destroy_bitmap(_bitmap);
}


void ScreenFade::Initialize()
{
	_start_time = _end_time = 0;
	_tint_color = al_map_rgb_f(1, 1, 1);

	_bitmap = al_load_bitmap(_bitmap_file);
	if(!_bitmap)
	{
		LOG_WRITE("White bitmap failed to load!");
	}

	_wid = al_get_bitmap_width(_bitmap);
	_hei = al_get_bitmap_height(_bitmap);
}

void ScreenFade::StartFade(double start_time, double end_time)
{
	_start_time = start_time;
	_end_time = end_time;
}

void ScreenFade::Update(double current_time)
{
	double alpha = (current_time - _start_time) / (_end_time - _start_time);
	alpha = max(0, alpha);
	alpha = min(1, alpha);

	_tint_color.a = alpha;

	if(alpha == 1)
	{
		al_draw_scaled_bitmap(_bitmap, 0, 0, _wid, _hei, 0,0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
		World::theWorld->Quit();
	}
	else
		al_draw_tinted_scaled_bitmap(_bitmap, _tint_color, 0, 0, _wid, _hei, 0,0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
}