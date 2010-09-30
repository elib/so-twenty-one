#pragma once

#include "Gameobject.h"
#include <vector>
using namespace std;

#include "TinyXml.h"

class Map
{
public:
	Map(void);
	~Map(void);

	static const int BackgroundLayer;
	static const int ForegroundLayer;

	bool Initialize(int offset_x, int offset_y);
	void Destroy();
	void Update(double delta_time, int index);

	void Collide(GameObject *otherobj);

	void OnHit();

	
	Vec2 spawnPointLocation;

protected:
	//resources:
	static const char *_mapSourceFile;
	static const char *_tileImageSourceFile[2];

	void LoadAvailableTiles(int index);
	void LoadTilesForLayer(TiXmlElement *layer_element, int index);
	
	//objects and their bitmaps
	vector<GameObject*> _tileObjects[2];
	//vector<GameObject*> _bgTileObjects;
	vector<ALLEGRO_BITMAP*> _availableBitmaps[2];
	Vec2 _offset;

	ALLEGRO_BITMAP *_largeBitmap[2];

	//map info
	int _width, _height, _tileWidth, _tileHeight;

	//big bitmap info - how many tiles is it?
	int _big_tiles_wide;
	int _big_tiles_high;
};