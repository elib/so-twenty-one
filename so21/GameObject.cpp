#include "GameObject.h"
#include "World.h"
#include <allegro5/allegro.h>


GameObject::GameObject(const char* filename, double x, double y)
{
	_bitmap = NULL;
	strcpy_s(_filename, 1024, filename);
	_position = Vec2(x, y);
}

void GameObject::Update(double delta_time)
{
	//find relative position to viewport
	Vec2 rel_pos = World::theWorld->TranslateToScreen(_position);

	//blit
	if(_bitmap != NULL)
	{
		al_draw_rotated_scaled_bitmap(_bitmap, 0, 0, rel_pos[0], rel_pos[1], 2, 2, 0, 0);
		//al_draw_bitmap(_bitmap, rel_pos[0], rel_pos[1], 0);
	}
}

void GameObject::Initialize()
{
	//load bitmap here
	_bitmap = al_load_bitmap(_filename);
	if(!_bitmap)
	{
		int err = al_get_errno();
	}
}

void GameObject::DestroyBitmap()
{
	if(_bitmap != NULL)
		al_destroy_bitmap(_bitmap);
	_bitmap = NULL;
}

GameObject::~GameObject(void)
{
	DestroyBitmap();
}