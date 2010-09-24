#pragma once

#include <vector>
#include "fonts.h"

using namespace std;

typedef struct TEXTSTRUCT
{
	TEXTSTRUCT(void)
	{
		memset((void*)msg, 0, sizeof(char) * 100);
	}

	char msg[100];
	ALLEGRO_FONT *font;
	ALLEGRO_COLOR color;
	int x, y;
	int flags;
} TextStruct;

class TextLayer
{
public:
	TextLayer(void);
	~TextLayer(void);

	void Update();
	void Initialize();

	void AddText(const char *message, ALLEGRO_FONT *font_size, ALLEGRO_COLOR color, int x, int y, int flags);
protected:
	vector<TextStruct> _texts;
};