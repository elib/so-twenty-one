#include "Fonts.h"

const char* Fonts::font_file( "resources/verdana.ttf" );

Fonts::Fonts(void)
{
}

Fonts::~Fonts(void)
{
}

bool Fonts::Initialize()
{
	// Load a font
	MediumFont = al_load_ttf_font(font_file, 48, 0);
	HugeFont = al_load_ttf_font(font_file, -100, 0);
	SmallFont = al_load_ttf_font(font_file, 10, 0);
	if(!MediumFont || !HugeFont || !SmallFont)
	{
		//printf("Error loading \"%s\".\n", font_file);
		//al_dis
		return false;
	}

	return true;
}