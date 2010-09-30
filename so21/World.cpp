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

	for(i = 0; i < NUM_DEBUG_CIRCLES; i++)
	{
		delete _debugCircles[i];
	}
}


bool World::Initialize(ALLEGRO_DISPLAY * display)
{
	_display = display;

	Keys.Initialize();

	LOG_WRITE("Starting...");

	if(!fonts.Initialize())
		return false;

	unsigned int i;
	for(i = 0; i < NUM_DEBUG_CIRCLES; i++)
	{
		double x = DISPLAY_WIDTH - 32;
		double y = (32* i);
		_debugCircles[i] = new Fader(x, y);
		_debugCircles[i]->Initialize();
		_debugCircles[i]->scrollFactor = Vec2(0.0, 0.0);
	}

	musicProvider.Initialize(_display);

	//place map in correct location
	_map.Initialize(0, 0);

	//we know where to put the player after initializing the map
	_player = new Player(_map.spawnPointLocation[0], _map.spawnPointLocation[1]);
	_player->Initialize();
	_gameObjects.push_back(_player);

	//map has loaded spawn point, so add that too
	_spawnpoint = new SpawnPoint(_map.spawnPointLocation[0], _map.spawnPointLocation[1]);
	_spawnpoint->Initialize();
	_gameObjects.push_back(_spawnpoint);

	_starfield.Initialize();

	//start from 0 now
	//_last_tick_count = GetTickCount();
	QueryPerformanceFrequency(&_performance_freq);
	QueryPerformanceCounter(&_last_tick_count);

	return true;
}

void World::Update()
{
	unsigned int i;

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

		_starfield.Update(delta);

		//update map
		_map.Update(delta);

		//update all subservient objects
		for(i = 0; i < _gameObjects.size(); i++)
		{
			((GameObject*)_gameObjects.at(i))->Update(delta);
		}

		_map.Collide(_player);


#ifdef DEBUG_RECORDING
		for(i = 0; i < musicProvider.eventsForCurrentFrame.size(); i++)
		{
			unsigned int type = musicProvider.eventsForCurrentFrame[i].type;
			if(type < 10)
			{
				_debugCircles[type]->Pulse();
			}
		}

		//update all debug circles
		for(i = 0; i < NUM_DEBUG_CIRCLES; i++)
		{
			_debugCircles[i]->Update(delta);
		}
#endif


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

	} //END hasfocus
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
	Vec2 modifiedCamPos = cameraPosition;
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