#include "TextLayer.h"

TextLayer::TextLayer(void)
{
}

TextLayer::~TextLayer(void)
{
}


void TextLayer::Update()
{
	unsigned int i;
	for(i = 0; i < _texts.size(); i++)
	{
		al_draw_text(_texts[i].font, _texts[i].color, _texts[i].x, _texts[i].y, _texts[i].flags, _texts[i].msg);
	}
	//when done, clear texts.
	_texts.clear();
}

void TextLayer::Initialize()
{
	//not sure anything to do here.
}

void TextLayer::AddText(const char *message, ALLEGRO_FONT *font_size, ALLEGRO_COLOR color, int x, int y, int flags)
{
	TextStruct newText;
	newText.flags = flags;
	newText.font = font_size;
	newText.x = x;
	newText.y = y;
	newText.color = color;
	strcat_s(newText.msg, message);

	_texts.push_back(newText);
}