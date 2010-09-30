#pragma once

#include <allegro5/allegro5.h>

#include "Fonts.h"
#include <vector>
#include "GameObject.h"
#include "Log.h"

#include "Keyboard.h"

#include "svl/svl.h"
#include "player.h"

#include "Map.h"
#include "MusicProvider.h"

#include "textlayer.h"

#include "Fader.h"

#include "StarField.h"

#include "SpawnPoint.h"

#define EPSILON (1E-6)

#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT	480

#define NUM_DEBUG_CIRCLES 10

#define LOG_WRITE(__msg_format, ...) { char __final_msg[1024]; sprintf_s(__final_msg, __msg_format "\r\n", __VA_ARGS__); World::theWorld->log.LogWrite(__final_msg); }

using namespace std;

class World
{
public:
	World(void);
	~World(void);

	bool Initialize(ALLEGRO_DISPLAY * display);
	void Update();

	Vec2 TranslateToScreen(Vec2 _position, Vec2 _scrollfactor);

	static World *theWorld;
	static ALLEGRO_DISPLAY *TheDisplay;

	void SwitchIn();
	void SwitchOut();
	
	Log log;

	Vec2 cameraPosition;

	Keyboard Keys;

	Fonts fonts;

	bool hasfocus;

	TextLayer textLayer;

	MusicProvider musicProvider;

protected:

	Player *_player;

	ALLEGRO_DISPLAY * _display;

	LARGE_INTEGER _performance_freq;
	LARGE_INTEGER _last_tick_count;

	void MoveCamera(double delta);

	void RemoveGameObjects();

	vector<GameObject*> _gameObjects;

	Fader *_debugCircles[NUM_DEBUG_CIRCLES];

	Map _map;
	SpawnPoint *_spawnpoint;

	StarField _starfield;

};