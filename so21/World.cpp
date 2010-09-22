#include "World.h"
#include <stdio.h>

World* World::theWorld = NULL;

World::World(void)
{
	theWorld = this;
}

World::~World(void)
{
	RemoveGameObjects();
}

void World::RemoveGameObjects()
{
	unsigned int i;
	for(i = 0; i < _gameObjects.size(); i++)
		delete (_gameObjects.at(i));
}


bool World::Initialize(ALLEGRO_TIMER *timer)
{

	log.Initialize();

	log.LogWrite("Starting ...");

	//init local
	_timer = timer;

	//get timer info
	_timer_speed = al_get_timer_speed(_timer);
	_tick_count = al_get_timer_count(_timer);

	if(!_fonts.Initialize())
		return false;

	GameObject *gameObj = new GameObject("Resources/probe-sprite.png", 40, 50);
	gameObj->Initialize();
	_gameObjects.push_back(gameObj);

	return true;
}

void World::Update()
{
	//get this frame's tick
	int64_t current_tick = al_get_timer_count(_timer);
	int ticks_diff = current_tick - _tick_count;

	//delta holds the frame's time in seconds.
	double delta = ticks_diff * _timer_speed;
	_tick_count = current_tick;

	al_clear_to_color(al_map_rgba(100, 100, 150, 255));

	float fps = (1.0 / delta);
	//print framerate
	char rate[50];
	sprintf_s(rate, 50, "FPS %f", fps);
	al_draw_text(_fonts.SmallFont, al_map_rgba_f(0.5, 1.0, 0.5, 1.0), 0, 0, -1, rate );

	unsigned int i;
	for(i = 0; i < _gameObjects.size(); i++)
	{
		((GameObject*)_gameObjects.at(i))->Update(delta);
	}
}