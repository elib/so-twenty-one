#include "GameObject.h"


GameObject::GameObject(const char* filename, double x, double y)
{
	_bitmap = NULL;
	strcpy_s(_filename, 1024, filename);
	_position = Vec2(x, y);
}

void GameObject::Update(double delta_time)
{
	if(_bitmap != NULL)
	{
		al_draw_bitmap(_bitmap, _position[0], _position[1], 0);
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
