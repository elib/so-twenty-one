#include "Map.h"
#include "World.h"
#include "tinyxml.h"

const char* Map::_mapSourceFile = "Resources/map.tmx";
const char* Map::_tileImageSourceFile[2] = {"Resources/tiles.png", "Resources/tiles_background.png"};

const int Map::BackgroundLayer = 1;
const int Map::ForegroundLayer = 0;

#define XML_TYPE_SPAWN	1

Map::Map(void)
{
	_width = _height = _tileWidth = _tileHeight = 0;
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
			gid = gid - _height*_width*index;
			if(gid > 0)
			{
				LOG_WRITE("Adding tile gid: %d to location (%d,%d)", gid, i, j);
				GameObject *obj = new GameObject(_availableBitmaps[index][gid - 1], i*(_tileWidth) + _offset[0], j*(_tileHeight) + _offset[1]);
				obj->Initialize();
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
	LoadAvailableTiles(BackgroundLayer);
	LoadAvailableTiles(ForegroundLayer);

	//back to XML - now load all tiles and place them in our vector
	TiXmlElement *layer_element = root->FirstChildElement("layer");
	if(!layer_element)
	{
		LOG_WRITE("Missing Layer element in XML!");
		return false;
	}

	LoadTilesForLayer(layer_element, ForegroundLayer);

	//get background ones
	layer_element = layer_element->NextSiblingElement("layer");

	LoadTilesForLayer(layer_element, BackgroundLayer);
	
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

		switch(type)
		{
		case XML_TYPE_SPAWN:
			{
				//resolve to tile-width positions
				x = x / _tileWidth;
				x = x * _tileWidth;
				y = y / _tileHeight;
				y = y * _tileHeight;

				spawnPointLocation = Vec2(x, y);
				LOG_WRITE("Spawnpoint at (%d,%d)", x, y);
			}
		}
		cur_element = cur_element->NextSiblingElement();
	}


	return true;
}

void Map::LoadAvailableTiles(int index)
{
	LOG_WRITE("Loading tile bitmaps from path: %s", _tileImageSourceFile[index]);
	_largeBitmap[index] = al_load_bitmap(_tileImageSourceFile[index]);
	int big_width = al_get_bitmap_width(_largeBitmap[index]);
	int big_height = al_get_bitmap_height(_largeBitmap[index]);
	int tiles_wide = big_width / _tileWidth;
	int tiles_high = big_height / _tileHeight;
	int i, j;

	//note backwards - this loads the tiles in the same order as the tiled program
	for(j = 0; j < tiles_high; j++)
	{
		for(i = 0; i < tiles_wide; i++)
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
	for(j = 0; j < 2; j++)
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
	for(i = 0; i < _tileObjects[ForegroundLayer].size(); i++)
	{
		otherobj->Collide(_tileObjects[ForegroundLayer][i]);
	}
}