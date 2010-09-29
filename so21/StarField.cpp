#include "StarField.h"

#include "World.h"

const char *StarField::_starsImageSourceFile = "Resources/stars.png";

//size of each individual star
const int StarField::_starsize = 16;

//not using all of the large bitmap.
const int StarField::_starcount = 8;

#define NUM_OF_STARS		(150)

#define MIN_ALPHA_DIV		(100)
#define MAX_ALPHA_DIV		(900)

#define MIN_SCROLL_DIV		(350)
#define MAX_SCROLL_DIV		(700)

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
		int wrand = (rand() % (DISPLAY_WIDTH + DISPLAY_WIDTH/2 + _starsize)) - _starsize/2 ;
		int hrand = (rand() % (DISPLAY_HEIGHT + _starsize)) - _starsize/2;
		int starrand = rand() % _starcount;

		int alpharand = (rand() % (MAX_ALPHA_DIV - MIN_ALPHA_DIV)) + MIN_ALPHA_DIV;
		double alpha = ((double)alpharand) / (1000.0) ;

		//about 3 in 10 stars will start stationary until the end.
		int stand_still = rand() % 10;
		double scrollfactor = 0;
		if(stand_still > 3)
		{
			//the rest - have a larger scroll factor
			int scrollrand = (rand() % (MAX_SCROLL_DIV - MIN_SCROLL_DIV)) + MIN_SCROLL_DIV;
			scrollfactor = ((double)scrollrand) / 1000.0;
		}
		else
		{
			//dim the further stars
			alpha = alpha / 3;
		}

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

	for(i = 0; i < _starLayer.size(); i++)
	{

		delete _starLayer[i];
	}


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
			//recycle object on right side of screen
			World::theWorld->cameraPosition[0];
			int wrand = (rand() % (DISPLAY_WIDTH/3)) + DISPLAY_WIDTH + World::theWorld->cameraPosition[0];
			int hrand = (rand() % (DISPLAY_HEIGHT + _starsize)) - _starsize/2;
			int starrand = rand() % _starcount;

			int scrollrand = (rand() % (MAX_SCROLL_DIV - MIN_SCROLL_DIV)) + MIN_SCROLL_DIV;
			double scrollfactor = ((double)scrollrand) / 1000.0;

			int alpharand = (rand() % (MAX_ALPHA_DIV - MIN_ALPHA_DIV)) + MIN_ALPHA_DIV;
			double alpha = ((double)alpharand) / (1000.0) ;

			//ALLEGRO_BITMAP *starbitmap = _availableBitmaps[starrand];
			_starLayer[i]->position = Vec2(wrand, hrand);
			_starLayer[i]->scrollFactor = Vec2(scrollfactor, 0.0);
			_starLayer[i]->alpha = alpha;
		}
	}
}