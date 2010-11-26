#pragma once

#include "Gameobject.h"
#include <vector>
using namespace std;

#include "TinyXml.h"

enum MapTypes
{
	MAP_BACKGROUND,
	MAP_FOREGROUND,
	MAP_FOREGROUND_COLLIDING,

	__MAP_TYPE_COUNT
};

class Map
{
public:
	Map(void);
	~Map(void);

	bool Initialize(int offset_x, int offset_y);
	void Destroy();
	void Update(double delta_time, int index);

	bool Collide(GameObject *otherobj);

	void OnHit();

	
	Vec2 spawnPointLocation;
	Vec2 titleLocation;

protected:
	//resources:
	static const char *_mapSourceFile;
	static const char *_tileImageSourceFile[__MAP_TYPE_COUNT];

	void LoadAvailableTiles(int index);
	void LoadTilesForLayer(TiXmlElement *layer_element, int index);

	void ConvertTilesToPositions(int &x, int &y);
	
	//objects and their bitmaps
	vector<GameObject*> _tileObjects[__MAP_TYPE_COUNT];
	//vector<GameObject*> _bgTileObjects;
	vector<ALLEGRO_BITMAP*> _availableBitmaps[__MAP_TYPE_COUNT];
	Vec2 _offset;

	ALLEGRO_BITMAP *_largeBitmap[__MAP_TYPE_COUNT];

	//map info
	int _width, _height, _tileWidth, _tileHeight;

	//big bitmap info - how many tiles is it?
	int _big_tiles_wide;
	int _big_tiles_high;
};