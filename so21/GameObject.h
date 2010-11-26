#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "svl/svl.h"

#include "quickmax.h"

#include <vector>
using namespace std;

class GameObject
{
public:
	GameObject(double x, double y);
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

	//determine if we have just left the screen (off of left side)
	bool LeftScreen();

	//collisions and physical properties
	virtual bool Collide(GameObject *otherobj);
	Vec2 size;
	double sqrradius;
	Rect bounding_box;

	bool bounding_box_debug;

protected:
	void DestroyBitmap();

	char _filename[1024];
	ALLEGRO_BITMAP *_bitmap;

	bool _leftscreen;

	bool _has_bitmap;

	Vec2 _frame_rel_pos;

	//ANIMATION
	bool _is_animated;
	double _animation_time;
	vector<ALLEGRO_BITMAP*> _animation_frames;
	void InitializeAnimation(int frame_width, int fps);

	ALLEGRO_BITMAP* GetCurrentAnimationFrame();

private:
	void CalculatePhysics(double delta_time);
};
