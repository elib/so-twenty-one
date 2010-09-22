#pragma once

#include <allegro5/allegro5.h>

#include "Fonts.h"
#include <vector>
#include "GameObject.h"
#include "Log.h"

#include "svl/svl.h"


#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT	480

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
	
	Log log;

	Vec2 cameraPosition;

private:
	DWORD _last_tick_count;

	void MoveCamera(double delta);

	void RemoveGameObjects();

	Fonts _fonts;

	vector<GameObject*> _gameObjects;

};