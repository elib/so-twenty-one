#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


//static const char* *font_file = "resources/verdana.ttf";

class Fonts
{
public:
	Fonts(void);
	~Fonts(void);
	bool Initialize();
	ALLEGRO_FONT *MediumFont, *HugeFont, *SmallFont;
private:
	static const char *font_file;
};
