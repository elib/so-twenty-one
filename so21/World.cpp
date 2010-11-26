#include "World.h"
#include <stdio.h>
#include "player.h"

World* World::theWorld = NULL;
ALLEGRO_DISPLAY* World::TheDisplay = NULL;

//fadeout time in seconds - when dying
#define FADEOUT_TIME	4.0

#define SECONDS_PER_TICK (1.0 / 1000.0)

#define TARGET_CAMERA_SPEED		(60)



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
	total_time = 0;
	_player = NULL;

	_camera_speed = 0;
	_target_camera_speed_time = 0;
	_camera_speed_start = 0;
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
	_game_over = false;
	_quitgame = false;
	_screenFade.Initialize();

	show_boundingbox = false;
	player_collide_debug = false;

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

	_bomblauncher = new BombLauncher(LAUNCHABLE_FLASHING_BOMB);
	_bomblauncher->Initialize();
	
	_laserlauncher = new BombLauncher(LAUNCHABLE_LASER);
	_laserlauncher->Initialize();

	Title* title = new Title(_map.titleLocation[0], _map.titleLocation[1]);
	title->Initialize();
	_gameObjects.push_back(title);

	_starfield.Initialize();

	//start from 0 now
	//_last_tick_count = GetTickCount();
	QueryPerformanceFrequency(&_performance_freq);
	QueryPerformanceCounter(&_last_tick_count);

	musicProvider.PlayMusic();

	return true;
}

bool World::Update()
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
		//add this to time
		total_time += delta;

		if(_game_over)
		{
			//freeze everything, keep working
			delta = 0;
		}

		musicProvider.Update();

		//update "camera"
		MoveCamera(delta);

		_starfield.Update(delta);

		//update map background
		_map.Update(delta, MAP_BACKGROUND);
#ifdef BOUNDINGBOX_ALLOW
		if(Keys.keys_just_down[ALLEGRO_KEY_B])
			show_boundingbox = !show_boundingbox;
		if(Keys.keys_just_down[ALLEGRO_KEY_H])
			player_collide_debug = !player_collide_debug;
#endif

		//update all subservient objects
		for(i = 0; i < _gameObjects.size(); i++)
		{
			((GameObject*)_gameObjects.at(i))->Update(delta);
		}

		//update map
		_map.Update(delta, MAP_FOREGROUND);
		_map.Update(delta, MAP_FOREGROUND_COLLIDING);

#ifndef BOUNDINGBOX_ALLOW
		player_collide_debug = false;
#endif
		if(!player_collide_debug)
		{
			_map.Collide(_player);
			_spawnpoint->Collide(_player);
		}

		_bomblauncher->Update(delta);
		_laserlauncher->Update(delta);


#ifdef DEBUG_RECORDING
		for(i = 0; i < musicProvider.eventsForCurrentFrame.size(); i++)
		{
			unsigned int type = musicProvider.eventsForCurrentFrame[i].type;
			if(type < 10)
			{
				_debugCircles[type]->Pulse();
			}

			switch(type)
			{
			case 2:
				_bomblauncher->LaunchBomb();
				break;
			case 3:
				_laserlauncher->LaunchBomb();
				break;
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

		if(_game_over)
		{
			_screenFade.Update(total_time);
		}

	} //END hasfocus

	al_flip_display();

	if(_quitgame)
		return false;

	return true;
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
	//static const double speed = 25;
	if(total_time < _target_camera_speed_time)
	{
		_camera_speed = TARGET_CAMERA_SPEED * ease_in((total_time - _camera_speed_start) / (_target_camera_speed_time - _camera_speed_start));
	}
	else if(_camera_speed_start > 0) //kludge way to know we aren't in initial state
	{
		_camera_speed = TARGET_CAMERA_SPEED;
	}

	double amount = _camera_speed * delta;
	cameraPosition[0] += amount;
}

void World::PlayerLeftSpawn()
{
	if(_target_camera_speed_time == 0)
	{
		double time_to_loop_end = musicProvider.DoNotLoop();
		//keep it under wraps
		time_to_loop_end = MAX(4, time_to_loop_end);
		time_to_loop_end = MIN(8, time_to_loop_end);

		LOG_WRITE("We're on our way - chosen speedup time is %f", time_to_loop_end);

		_target_camera_speed_time = total_time + time_to_loop_end;
		_camera_speed_start = total_time;
	}
}

void World::LoseGame()
{
	if(!_game_over)
	{
		_game_over = true;
		_screenFade.StartFade(total_time, total_time + FADEOUT_TIME);
		musicProvider.Fadeout(FADEOUT_TIME);
	}
}

void World::Quit()
{
	_quitgame = true;
}

Vec2 World::PlayerPosition()
{
	return _player->position;
}