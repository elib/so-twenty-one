#include "Map.h"
#include "World.h"
#include "tinyxml.h"

const char* Map::_mapSourceFile = "Resources/map.tmx";
const char* Map::_tileImageSourceFile = "Resources/tiles.png";

Map::Map(void)
{
	_width = _height = _tileWidth = _tileHeight = 0;
}

Map::~Map(void)
{
	Destroy();
}

bool Map::Initialize(int offset_x, int offset_y)
{
	LOG_WRITE("Loading map data from path: %s", _mapSourceFile);

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
	LoadAvailableTiles();

	//back to XML - now load all objects
	TiXmlElement *layer_element = root->FirstChildElement("layer");
	if(!layer_element)
	{
		LOG_WRITE("Missing Layer element in XML!");
		return false;
	}

	TiXmlElement *datael = layer_element->FirstChildElement();
	TiXmlElement *cur_element = datael->FirstChildElement();
	int i, j;
	for(j = 0; j < _height; j++)
	{
		for(i = 0; i < _width; i++)
		{
			int gid;
			cur_element->QueryIntAttribute("gid", &gid);
			if(gid > 0)
			{
				LOG_WRITE("Adding tile gid: %d to location (%d,%d)", gid, i, j);
				Vec2 origin = ((Vec2)_tile_origins.at(gid -1 ));
				ALLEGRO_BITMAP* subbitmap = al_create_sub_bitmap(_largeBitmap, origin[0], origin[1], _tileWidth, _tileHeight);
				int x_ = origin[0];
				int y_ = origin[1];
				GameObject *obj = new GameObject(subbitmap, i*(_tileWidth) + _offset[0], j*(_tileHeight) + _offset[1]);
				obj->Initialize();
				_tileObjects.push_back(obj);
			}
			cur_element = cur_element->NextSiblingElement();
		}
	}
	return true;
}

void Map::LoadAvailableTiles()
{
	LOG_WRITE("Loading tile bitmaps from path: %s", _tileImageSourceFile);
	_largeBitmap = al_load_bitmap(_tileImageSourceFile);
	int big_width = al_get_bitmap_width(_largeBitmap);
	int big_height = al_get_bitmap_height(_largeBitmap);
	int tiles_wide = big_width / _tileWidth;
	int tiles_high = big_height / _tileHeight;
	int i, j;

	//note backwards - this loads the tiles in the same order as the tiled program
	for(j = 0; j < tiles_high; j++)
	{
		for(i = 0; i < tiles_wide; i++)
		{
			_tile_origins.push_back(Vec2(i*_tileWidth, j*_tileHeight));
		}
	}
}

void Map::Destroy()
{
	unsigned int i;
	for(i = 0; i < _tileObjects.size(); i++)
	{
		delete (_tileObjects.at(i));
	}
	_tileObjects.clear();

	_tile_origins.clear();

	al_destroy_bitmap(_largeBitmap);
}

void Map::Update(double delta_time)
{
	unsigned int i;
	for(i = 0; i < _tileObjects.size(); i++)
	{
		((GameObject*)_tileObjects.at(i))->Update(delta_time);
	}
}

void Map::Collide(GameObject *otherobj)
{
	unsigned int i;
	for(i = 0; i < _tileObjects.size(); i++)
	{
		_tileObjects[i]->Collide(otherobj);
	}
}