#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "svl/svl.h"

#include "quickmax.h"

//#include "bitmask-1.6a/bitmask.h"

class GameObject
{
public:
	GameObject(const char* filename, double x, double y);
	GameObject(ALLEGRO_BITMAP *bitmap, double x, double y);
	virtual ~GameObject(void);

	//override me!!!
	virtual void Initialize();
	virtual void Update(double delta_time);

	//movement
	Vec2 acceleration;
	Vec2 velocity;
	Vec2 position;
	double max_velocity;
	double damping;

	//graphics
	double alpha;
	bool visible;
	Vec2 scrollFactor;
	
	bool LeftScreen();

	Vec2 size;

	double sqrradius;

	Rect bounding_box;

	void Collide(GameObject *otherobj);

	//void GenerateMaskByAlpha();


	//bitmask_t *bitMask;

protected:
	void DestroyBitmap();

	char _filename[1024];
	ALLEGRO_BITMAP *_bitmap;

	bool _leftscreen;
};
