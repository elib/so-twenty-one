#include "World.h"
#include <stdio.h>
#include "player.h"




World* World::theWorld = NULL;
ALLEGRO_DISPLAY* World::TheDisplay = NULL;

#define SECONDS_PER_TICK (1.0 / 1000.0)

#ifdef _DEBUG

//when debugging, always show fps
#ifndef SHOW_FPS
#define SHOW_FPS
#endif

//when debugging, always debug recording
#ifndef DEBUG_RECORDING
#define DEBUG_RECORDING
#endif

#endif

World::World(void)
{
	//static member
	theWorld = this;
	hasfocus = true;

	//before anything else - even in constructor!
	log.Initialize();
	textLayer.Initialize();

	cameraPosition = Vec2(0.0,0.0);
	_last_tick_count.QuadPart = 0;
	_player = NULL;
}

World::~World(void)
{
	//player is in this list as well, no need to release
	RemoveGameObjects();
}

void World::RemoveGameObjects()
{
	unsigned int i;
	for(i = 0; i < _gameObjects.size(); i++)
		delete (_gameObjects.at(i));

	//player already deleted in above loop
	_player = NULL;
}


bool World::Initialize(ALLEGRO_DISPLAY * display)
{
	_display = display;

	Keys.Initialize();

	LOG_WRITE("Starting...");

	if(!fonts.Initialize())
		return false;

	_player = new Player(0, 0);
	_player->Initialize();
	_gameObjects.push_back(_player);

	_debugCircle = new GameObject("Resources/debug_circle.png", DISPLAY_WIDTH - 32, 0);
	_debugCircle->Initialize();
	_debugCircle->scrollFactor = Vec2(0.0, 0.0);
	_debugCircle->visible = false;
	_gameObjects.push_back(_debugCircle);


	musicProvider.Initialize(_display);

	//place map in correct location
	_map.Initialize(-DISPLAY_WIDTH/2, -DISPLAY_HEIGHT/2);

	//start from 0 now
	//_last_tick_count = GetTickCount();
	QueryPerformanceFrequency(&_performance_freq);
	QueryPerformanceCounter(&_last_tick_count);

	return true;
}

void World::Update()
{
	//get this frame's tick
	LARGE_INTEGER current_tick;
	QueryPerformanceCounter(&current_tick);
	LONGLONG ticks_diff = current_tick.QuadPart - _last_tick_count.QuadPart;

	//delta holds the frame's time in seconds.
	double delta = ticks_diff / ((double) _performance_freq.QuadPart);
	_last_tick_count.QuadPart = current_tick.QuadPart;

	//start frame
	al_clear_to_color(al_map_rgba(10, 10, 50, 255));

	if(hasfocus)
	{
		musicProvider.Update();

		//update "camera"
		MoveCamera(delta);

		//update map
		_map.Update(delta);

#ifdef DEBUG_RECORDING
		if(musicProvider.eventsForCurrentFrame.size() > 0)
		{
			_debugCircle->visible = true;
		}
		else
		{
			_debugCircle->visible = false;
		}
#endif

		//update all subservient objects
		unsigned int i;
		for(i = 0; i < _gameObjects.size(); i++)
		{
			((GameObject*)_gameObjects.at(i))->Update(delta);
		}

#ifdef SHOW_FPS
		//print framerate
		float fps = (1.0 / delta);
		char rate[50];
		sprintf_s(rate, "FPS %f", fps);
		textLayer.AddText(rate, fonts.SmallFont, al_map_rgba_f(0.9, 1.0, 0.2, 1.0), 0, 0, ALLEGRO_ALIGN_LEFT);
#endif

		//draw texts
		textLayer.Update();

		//at the end, update keyboard state to ready for next frame
		Keys.Update();
	}
}

void World::SwitchIn()
{
	hasfocus = true;
	musicProvider.PlayMusic();
}

void World::SwitchOut()
{
	hasfocus = false;
	musicProvider.StopMusic();
}


Vec2 World::TranslateToScreen(Vec2 _position, Vec2 _scrollfactor)
{
	static const Vec2 half_screen(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
	Vec2 modifiedCamPos = cameraPosition - half_screen;
	modifiedCamPos[0] *= _scrollfactor[0];
	modifiedCamPos[1] *= _scrollfactor[1];

	return _position - modifiedCamPos;
}

void World::MoveCamera(double delta)
{
	//moving right constantly
	static const double speed = 25;
	double amount = speed * delta;
	cameraPosition[0] += amount;
}