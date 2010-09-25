#include "GameObject.h"
#include "World.h"
#include <allegro5/allegro.h>

#include "quickmax.h"


GameObject::GameObject(const char* filename, double x, double y)
{
	_bitmap = NULL;
	strcpy_s(_filename, 1024, filename);
	position = Vec2(x, y);
}

GameObject::GameObject(ALLEGRO_BITMAP *bitmap, double x, double y)
{
	position = Vec2(x, y);
	strcpy_s(_filename, 1024, "[MEMORY]");
	_bitmap = bitmap;
}

void GameObject::Update(double delta_time)
{
	_leftscreen = false;

	//"physics" time!
	Vec2 calculation_accel = acceleration;
	if(sqrlen(velocity) > EPSILON)
	{
		calculation_accel -= norm(velocity) * damping;
	}

	velocity += calculation_accel * delta_time;

	if(sqrlen(velocity) > sqr(max_velocity))
	{
		velocity = norm(velocity) * max_velocity;
	}

	position += velocity * delta_time;
	

	//find relative position to viewport
	Vec2 rel_pos = World::theWorld->TranslateToScreen(position, scrollFactor);
	if(rel_pos[0] + size[0] < 0)
		_leftscreen = true;

	alpha = MAX(alpha, 0.0);
	alpha = MIN(alpha, 1.0);

	//blit
	if(_bitmap != NULL)
	{
		if(visible)
		{
			//don't draw if alpha is very small
			if(alpha > EPSILON)
			{
				//if alpha is about 1, draw regularly
				if(alpha > (1.0 - EPSILON)) //not sure if this does what I think it does
				{
					//al_draw_rotated_scaled_bitmap(_bitmap, 0, 0, rel_pos[0], rel_pos[1], 1, 1, 0, 0);
					al_draw_bitmap(_bitmap, rel_pos[0], rel_pos[1], 0);
				}
				else
				{
					al_draw_tinted_bitmap(_bitmap, al_map_rgba_f(1.0, 1.0, 1.0, alpha),
						rel_pos[0], rel_pos[1], 0);
				}
			}
		}
	}
}

void GameObject::Initialize()
{
	acceleration = velocity = Vec2(0.0, 0.0);
	max_velocity = 100;
	damping = 0.1;

	alpha = 1.0;

	visible = true;
	scrollFactor = Vec2(1.0, 1.0);

	_leftscreen = false;

	//load bitmap here
	if(!_bitmap)
	{
		LOG_WRITE("Creating bitmap: %s.", _filename);
		_bitmap = al_load_bitmap(_filename);

		if(!_bitmap)
		{
			int err = al_get_errno();
			LOG_WRITE("Error creating bitmap! Filename: %s, error number: %d.", _filename, err);
		}
	}

	size[0] = al_get_bitmap_width(_bitmap);
	size[1] = al_get_bitmap_height(_bitmap);
}

void GameObject::DestroyBitmap()
{
	if(_bitmap != NULL)
	{
		LOG_WRITE("Destroying bitmap: %s.", _filename);
		al_destroy_bitmap(_bitmap);
	}
	_bitmap = NULL;
}

GameObject::~GameObject(void)
{
	DestroyBitmap();
}

bool GameObject::LeftScreen()
{
	return _leftscreen;
}