#include "Map.h"
#include "World.h"
#include "tinyxml.h"

const char* Map::_mapSourceFile = "Resources/map.tmx";
const char* Map::_tileImageSourceFile[__MAP_TYPE_COUNT] = {
					"Resources/tiles_background.png",
					"Resources/tiles_foreground.png",
					"Resources/tiles.png"};

#define XML_TYPE_SPAWN	1
#define XML_TYPE_TITLE	2

Map::Map(void)
{
	_width = _height = _tileWidth = _tileHeight = _big_tiles_wide = _big_tiles_high = 0;
}

Map::~Map(void)
{
	Destroy();
}

void Map::LoadTilesForLayer(TiXmlElement *layer_element, int index)
{
	TiXmlElement *datael = layer_element->FirstChildElement();
	TiXmlElement *cur_element = datael->FirstChildElement();
	int i, j;
	for(j = 0; j < _height; j++)
	{
		for(i = 0; i < _width; i++)
		{
			int gid;
			cur_element->QueryIntAttribute("gid", &gid);
			//each layer starts with a new base GID
			gid = (gid % (_big_tiles_wide*_big_tiles_high));
			if(gid > 0)
			{
				LOG_WRITE("Adding tile gid: %d to location (%d,%d)", gid, i, j);
				GameObject *obj = new GameObject(_availableBitmaps[index][gid - 1], i*(_tileWidth) + _offset[0], j*(_tileHeight) + _offset[1]);
				obj->Initialize();
				if(index == MAP_BACKGROUND)
				{
					//make it lag 
					obj->scrollFactor[0] = 0.3;
				}
				_tileObjects[index].push_back(obj);
			}
			cur_element = cur_element->NextSiblingElement();
		}
	}
}

bool Map::Initialize(int offset_x, int offset_y)
{
	LOG_WRITE("Loading map data from path: %s", _mapSourceFile);

	spawnPointLocation = Vec2(0.0, 0.0);

	_offset = Vec2(offset_x, offset_y);

	TiXmlDocument doc(_mapSourceFile);
	if (!doc.LoadFile())
	{
		LOG_WRITE("Error loading map: %s", doc.ErrorDesc());
		return false;
	}

	TiXmlElement *root = doc.RootElement();

	root->QueryIntAttribute("width", &_width);
	root->QueryIntAttribute("height", &_height); 
	root->QueryIntAttribute("tilewidth", &_tileWidth);
	root->QueryIntAttribute("tileheight", &_tileHeight);

	LOG_WRITE("Map size: (%d x %d), tile size: (%d x %d)", _width, _height, _tileWidth, _tileHeight);

	//now we take a break from XML and load bitmaps
	LoadAvailableTiles(MAP_BACKGROUND);
	LoadAvailableTiles(MAP_FOREGROUND);
	LoadAvailableTiles(MAP_FOREGROUND_COLLIDING);

	//back to XML - now load all tiles and place them in our vector
	TiXmlElement *layer_element = root->FirstChildElement("layer");
	if(!layer_element)
	{
		LOG_WRITE("Missing Layer element in XML!");
		return false;
	}

	//get background ones
	LoadTilesForLayer(layer_element, MAP_BACKGROUND);
	
	//foreground
	layer_element = layer_element->NextSiblingElement("layer");
	LoadTilesForLayer(layer_element, MAP_FOREGROUND);

	//foreground colliding
	layer_element = layer_element->NextSiblingElement("layer");
	LoadTilesForLayer(layer_element, MAP_FOREGROUND_COLLIDING);
	
	
	//now load all objects
	TiXmlElement *object_element = root->FirstChildElement("objectgroup");
	if(!object_element)
	{
		LOG_WRITE("Missing ObjectGroup element in XML!");
		return false;
	}

	/*
	get objects - this one is the spawn point

	<objectgroup name="Objects" width="200" height="15">
		<object name="" x="276" y="209"/>
	</objectgroup>*/

	//run through elements
	TiXmlElement *cur_element = object_element->FirstChildElement();
	while(cur_element != NULL)
	{
		int type, x, y;
		cur_element->QueryIntAttribute("type", &type);
		cur_element->QueryIntAttribute("x", &x);
		cur_element->QueryIntAttribute("y", &y);
		ConvertTilesToPositions(x, y);

		switch(type)
		{
		case XML_TYPE_SPAWN:
			{
				spawnPointLocation = Vec2(x, y);
				LOG_WRITE("Spawnpoint at (%f,%f)", x, y);
			}
		case XML_TYPE_TITLE:
			{
				//titleLocation
				titleLocation = Vec2(x, y);
				LOG_WRITE("Title at (%f, %f)", x, y);
			}
		}
		cur_element = cur_element->NextSiblingElement();
	}


	return true;
}

void Map::ConvertTilesToPositions(int &x, int &y)
{
	//resolve to tile-width positions
	x = x / _tileWidth;
	x = x * _tileWidth;
	y = y / _tileHeight;
	y = y * _tileHeight;
}

void Map::LoadAvailableTiles(int index)
{
	LOG_WRITE("Loading tile bitmaps from path: %s", _tileImageSourceFile[index]);
	_largeBitmap[index] = al_load_bitmap(_tileImageSourceFile[index]);
	int big_width = al_get_bitmap_width(_largeBitmap[index]);
	int big_height = al_get_bitmap_height(_largeBitmap[index]);
	_big_tiles_wide = big_width / _tileWidth;
	_big_tiles_high = big_height / _tileHeight;
	int i, j;

	//note backwards - this loads the tiles in the same order as the tiled program
	for(j = 0; j < _big_tiles_wide; j++)
	{
		for(i = 0; i < _big_tiles_high; i++)
		{
			ALLEGRO_BITMAP* subbitmap = al_create_sub_bitmap(_largeBitmap[index], i*_tileWidth, j*_tileHeight,
					_tileWidth, _tileHeight);
			_availableBitmaps[index].push_back(subbitmap);
		}
	}
}

void Map::Destroy()
{
	LOG_WRITE("Destroying map bitmaps and objects.");

	unsigned int i, j;
	for(j = 0; j < __MAP_TYPE_COUNT; j++)
	{
		for(i = 0; i < _tileObjects[j].size(); i++)
		{
			delete (_tileObjects[j][i]);
		}
		_tileObjects[j].clear();

		for(i = 0; i < _availableBitmaps[j].size(); i++)
		{
			al_destroy_bitmap(_availableBitmaps[j][i]);
		}

		al_destroy_bitmap(_largeBitmap[j]);
	}

}

void Map::Update(double delta_time, int index)
{
	unsigned int i;
	for(i = 0; i < _tileObjects[index].size(); i++)
	{
		_tileObjects[index][i]->Update(delta_time);
	}
}

void Map::Collide(GameObject *otherobj)
{
	unsigned int i;
	for(i = 0; i < _tileObjects[MAP_FOREGROUND_COLLIDING].size(); i++)
	{
		otherobj->Collide(_tileObjects[MAP_FOREGROUND_COLLIDING][i]);
	}
}