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

#define EPSILON (1E-6)


#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT	480


#define LOG_WRITE(__msg_format, ...) { char __final_msg[1024]; sprintf_s(__final_msg, 1024, __msg_format "\r\n", __VA_ARGS__); World::theWorld->log.LogWrite(__final_msg); }

using namespace std;

class World
{
public:
	World(void);
	~World(void);

	bool Initialize();
	void Update();

	Vec2 TranslateToScreen(Vec2 _position);

	static World *theWorld;
	static ALLEGRO_DISPLAY *TheDisplay;
	
	Log log;

	Vec2 cameraPosition;

	Keyboard Keys;

	Fonts fonts;

	bool hasfocus;

	TextLayer textLayer;

protected:

	Player *_player;

	LARGE_INTEGER _performance_freq;
	LARGE_INTEGER _last_tick_count;

	void MoveCamera(double delta);

	void RemoveGameObjects();

	vector<GameObject*> _gameObjects;

	Map _map;

};