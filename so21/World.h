#pragma once

#include <allegro5/allegro5.h>

#include "Fonts.h"
#include <vector>
#include "GameObject.h"
#include "Log.h"

using namespace std;

class World
{
public:
	World(void);
	~World(void);

	bool Initialize(ALLEGRO_TIMER *timer);
	void Update();

	static World *theWorld;
	
	Log log;

private:
	ALLEGRO_TIMER *_timer;
	double _timer_speed;
	int64_t _tick_count;

	void RemoveGameObjects();

	Fonts _fonts;

	vector<GameObject*> _gameObjects;

};