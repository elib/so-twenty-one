#include "GameObject.h"
#include "World.h"
#include <allegro5/allegro.h>

GameObject::GameObject(double x, double y)
{
	//no-bitmap mode
	_bitmap = NULL;
	_has_bitmap = false;
	strcpy_s(_filename, "[NO BITMAP]");
	position = Vec2(x, y);
}

GameObject::GameObject(const char* filename, double x, double y)
{
	_bitmap = NULL;
	_has_bitmap = true;
	strcpy_s(_filename, filename);
	position = Vec2(x, y);
}

GameObject::GameObject(ALLEGRO_BITMAP *bitmap, double x, double y)
{
	position = Vec2(x, y);
	_has_bitmap = true;
	strcpy_s(_filename, "[MEMORY]");
	_bitmap = bitmap;
}


void GameObject::Update(double delta_time)
{
	_leftscreen = false;
	_frame_rel_pos = Vec2(0.0,0.0);

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
	_frame_rel_pos = World::theWorld->TranslateToScreen(position, scrollFactor);
	if(_frame_rel_pos[0] + size[0] < 0)
		_leftscreen = true;

	alpha = MAX(alpha, 0.0);
	alpha = MIN(alpha, 1.0);

	//blit
	if(!_has_bitmap)
		return;

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
					al_draw_bitmap(_bitmap, _frame_rel_pos[0], _frame_rel_pos[1], 0);
				}
				else
				{
					al_draw_tinted_bitmap(_bitmap, al_map_rgba_f(1.0, 1.0, 1.0, alpha),
						_frame_rel_pos[0], _frame_rel_pos[1], 0);
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
	if(_has_bitmap)
	{
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

		size[0] = (double)al_get_bitmap_width(_bitmap);
		size[1] = (double)al_get_bitmap_height(_bitmap);
	}
	else //no bitmap
	{
		size[0] = size[1] = 1.0;
	}


	sqrradius = sqrlen(size);

	bounding_box.x = bounding_box.y = 0;
	bounding_box.width = size[0];
	bounding_box.height = size[1];
}

void GameObject::DestroyBitmap()
{
	if(_has_bitmap)
	{
		if(_bitmap != NULL)
		{
			if(!al_is_sub_bitmap(_bitmap))
			{
				LOG_WRITE("Destroying bitmap: %s.", _filename);
				al_destroy_bitmap(_bitmap);
			}
		}
	}
	_bitmap = NULL;
}

GameObject::~GameObject(void)
{
	DestroyBitmap();

	//bitmask_free(bitMask);
	//bitMask = NULL;
}

bool GameObject::LeftScreen()
{
	return _leftscreen;
}

void GameObject::Collide(GameObject *otherobj)
{
	Vec2 offset(otherobj->position - this->position);

	//check proximity
	if(sqrlen(offset) > MAX(this->sqrradius, otherobj->sqrradius))
		return;

	//copy rects for comfort
	Rect a = this->bounding_box;
	Rect b = otherobj->bounding_box;

	//translate to origin of "a" bounding box
	offset[0] -= a.x;
	offset[1] -= a.y;

	//translate "b" bounding box in terms of "a"
	b.x -= offset[0];
	b.y -= offset[1];

	if((fabs(a.x - b.x) < (a.width + b.width) / 2) &&
		(fabs(a.y - b.y) < (a.height + b.height) / 2))
	{
		double x_overlap = ((a.width + b.width) / 2) - fabs(a.x - b.x);
		double y_overlap = ((a.height + b.height) / 2) - fabs(a.y - b.y);

		Vec2 correction = Vec2(0.0, 0.0);

		if(x_overlap < y_overlap)
		{
			//determine if we're on left or right
			if((b.x - a.x) > 0)
			{
				//moving left moves away
				correction[0] = x_overlap;
			}
			else
			{
				correction[0] = - x_overlap;
			}
			
		}
		else //y overlap is lesser of the two
		{
			//determine if we're on top or bottom
			if((b.y - a.y) > 0)
			{
				//moving up moves away
				correction[1] = y_overlap;
			}
			else
			{
				correction[1] = - y_overlap;
			}
		}

		this->position += correction;
	
		//LOG_WRITE("Collision at (%f, %f) - overlap is (%f, %f)", offset[0], offset[1], x_overlap, y_overlap);
	}
}