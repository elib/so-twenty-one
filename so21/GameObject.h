#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "svl/svl.h"

class GameObject
{
public:
	GameObject(const char* filename, double x, double y);
	~GameObject(void);

	void Initialize();
	void Update(double delta_time);

private:
	void DestroyBitmap();

	char _filename[1024];
	ALLEGRO_BITMAP *_bitmap;
	Vec2 _position;
};
