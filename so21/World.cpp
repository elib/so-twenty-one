#include "World.h"
#include <stdio.h>
#include "player.h"

World* World::theWorld = NULL;
ALLEGRO_DISPLAY* World::TheDisplay = NULL;

#define SECONDS_PER_TICK (1.0 / 1000.0)

World::World(void)
{
	//static member
	theWorld = this;

	cameraPosition = Vec2(0.0,0.0);
	_last_tick_count = 0;
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

	_player = NULL;
}


bool World::Initialize(ALLEGRO_DISPLAY *display)
{
	//this next line is horrible =(
	TheDisplay = display;

	Keys.Initialize();

	log.Initialize();

	LOG_WRITE("Starting...");

	if(!_fonts.Initialize())
		return false;

	_player = new Player(0, 0);
	_player->Initialize();
	_gameObjects.push_back(_player);

	//place map in correct location
	_map.Initialize(-DISPLAY_WIDTH/2, -DISPLAY_HEIGHT/2);

	//start from 0 now
	_last_tick_count = GetTickCount();

	return true;
}

void World::Update()
{
	//get this frame's tick
	DWORD current_tick = GetTickCount();
	DWORD ticks_diff = current_tick - _last_tick_count;

	//delta holds the frame's time in seconds.
	double delta = ticks_diff * SECONDS_PER_TICK;
	_last_tick_count = current_tick;

	al_clear_to_color(al_map_rgba(100, 100, 150, 255));

	float fps = (1.0 / delta);
	//print framerate
	char rate[50];
	sprintf_s(rate, 50, "FPS %f", fps);
	al_draw_text(_fonts.SmallFont, al_map_rgba_f(0.5, 1.0, 0.5, 1.0), 0, 0, -1, rate );

	MoveCamera(delta);

	//update map
	_map.Update(delta);

	unsigned int i;
	for(i = 0; i < _gameObjects.size(); i++)
	{
		((GameObject*)_gameObjects.at(i))->Update(delta);
	}
}

Vec2 World::TranslateToScreen(Vec2 _position)
{
	static const Vec2 half_screen(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
	return _position - cameraPosition + half_screen;
}

void World::MoveCamera(double delta)
{
	static const double speed = 20;
	double amount = speed * delta;
	cameraPosition[0] += amount;
}