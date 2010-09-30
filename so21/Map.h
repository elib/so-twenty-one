#pragma once

#include "Gameobject.h"
#include <vector>
using namespace std;

class Map
{
public:
	Map(void);
	~Map(void);

	bool Initialize(int offset_x, int offset_y);
	void Destroy();
	void Update(double delta_time);

	void Collide(GameObject *otherobj);

	void OnHit();

	
	Vec2 spawnPointLocation;

protected:
	//resources:
	static const char *_mapSourceFile;
	static const char *_tileImageSourceFile;

	void LoadAvailableTiles();
	
	//objects and their bitmaps
	vector<GameObject*> _tileObjects;
	vector<ALLEGRO_BITMAP*> _availableBitmaps;
	Vec2 _offset;

	ALLEGRO_BITMAP *_largeBitmap;

	//map info
	int _width, _height, _tileWidth, _tileHeight;
};