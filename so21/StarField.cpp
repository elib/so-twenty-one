#include "StarField.h"

#include "World.h"

const char *StarField::_starsImageSourceFile = "Resources/stars.png";

//size of each individual star
const int StarField::_starsize = 16;

//not using all of the large bitmap.
const int StarField::_starcount = 8;

#define NUM_OF_STARS		(100)

#define MIN_ALPHA_DIV		(100)
#define MAX_ALPHA_DIV		(900)

StarField::StarField(void)
{
}

StarField::~StarField(void)
{
	Destroy();
}


void StarField::Initialize()
{
	LoadAvailableBitmaps();

	//initial population
	unsigned int i;
	for(i = 0; i < NUM_OF_STARS; i++)
	{
		int wrand = (rand() % (DISPLAY_WIDTH + _starsize)) - _starsize/2;
		int hrand = (rand() % (DISPLAY_HEIGHT + _starsize)) - _starsize/2;
		int starrand = rand() % _starcount;

		int scrollrand = (rand() % 200);
		double scrollfactor = ((double)scrollrand) / 1000.0;


		int alpharand = (rand() % (MAX_ALPHA_DIV - MIN_ALPHA_DIV)) + MIN_ALPHA_DIV;
		double alpha = ((double)alpharand) / (1000.0) ;

		ALLEGRO_BITMAP *starbitmap = _availableBitmaps[starrand];

		GameObject *obj = new GameObject(starbitmap, wrand, hrand);
		obj->Initialize();
		obj->scrollFactor = Vec2(scrollfactor, 0.0);
		obj->alpha = alpha;
		_starLayer.push_back(obj);
	}

}

void StarField::LoadAvailableBitmaps()
{
	LOG_WRITE("Loading star map images");
	_largeBitmap = al_load_bitmap(_starsImageSourceFile);
	if(!_largeBitmap)
	{
		LOG_WRITE("Could not load star bitmap!");
		return;
	}

	int bitmapwidth = al_get_bitmap_width(_largeBitmap);
	//int bitmapheight = al_get_bitmap_height(_largeBitmap);

	unsigned int i;
	for(i = 0; i < _starcount; i++)
	{
		int row = i / bitmapwidth;
		int col = i % bitmapwidth;

		ALLEGRO_BITMAP *bitmap =
			al_create_sub_bitmap(_largeBitmap, col*_starsize, row*_starsize, _starsize, _starsize);
		_availableBitmaps.push_back(bitmap);
	}
}


void StarField::Destroy()
{
	unsigned int i;

	LOG_WRITE("Destroying star map images");

	//what happens when I destroy the game objects as well?? :S
	for(i = 0; i < _availableBitmaps.size(); i++)
	{
		al_destroy_bitmap(_availableBitmaps[i]);
	}

	//destroy parent too
	al_destroy_bitmap(_largeBitmap);
}

void StarField::Update(double delta_time)
{

	unsigned int i;

	for(i = 0; i < _starLayer.size(); i++)
	{
		_starLayer[i]->Update(delta_time);
		if(_starLayer[i]->LeftScreen())
		{

		}
	}
}