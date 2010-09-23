#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "svl/svl.h"

class GameObject
{
public:
	GameObject(const char* filename, double x, double y);
	GameObject(ALLEGRO_BITMAP *bitmap, double x, double y);
	virtual ~GameObject(void);

	virtual void Initialize();
	virtual void Update(double delta_time);

	Vec2 acceleration;
	Vec2 velocity;
	Vec2 position;
	double max_velocity;
	double damping;

protected:
	void DestroyBitmap();

	char _filename[1024];
	ALLEGRO_BITMAP *_bitmap;
};
