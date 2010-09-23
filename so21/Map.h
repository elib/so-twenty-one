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

protected:
	//resources:
	static const char *_mapSourceFile;
	static const char *_tileImageSourceFile;

	void LoadAvailableTiles();
	
	//objects and their bitmaps
	vector<GameObject*> _tileObjects;
	vector<ALLEGRO_BITMAP*> _tile_bitmaps;
	Vec2 _offset;

	//map info
	int _width, _height, _tileWidth, _tileHeight;
};