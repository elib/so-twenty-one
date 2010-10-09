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
#include "ScreenFade.h"

#include "FlashingBomb.h"

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
	bool Update();

	Vec2 TranslateToScreen(Vec2 _position, Vec2 _scrollfactor);

	static World *theWorld;
	static ALLEGRO_DISPLAY *TheDisplay;

	void SwitchIn();
	void SwitchOut();
	
	Log log;

	Vec2 cameraPosition;

	Keyboard Keys;

	Fonts fonts;

	double total_time;

	bool hasfocus;

	TextLayer textLayer;

	MusicProvider musicProvider;

	//"EVENT"!
	void PlayerLeftSpawn();

	void LoseGame();

	void Quit();


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

	double _camera_speed;

	double _target_camera_speed_time;
	double _camera_speed_start;

	bool _game_over;
	bool _quitgame;

	ScreenFade _screenFade;



	//test - remove me
	FlashingBomb *_flashingbomb;
};