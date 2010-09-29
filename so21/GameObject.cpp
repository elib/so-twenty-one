#include "GameObject.h"
#include "World.h"
#include <allegro5/allegro.h>

#include "math.h"

inline int round(double value)
{
	/* No, the case where value is negative is *NOT* a special case */
	return (int)floor(value+0.5);
}

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
//
//void GameObject::GenerateMaskByAlpha()
//{
//	//call this once, before using the collision mask.
//	//it should copy the alpha mask to the bit mask.
//
//	//clear bitmask
//	bitmask_clear(bitMask);
//
//	//lock bitmap
//	ALLEGRO_LOCKED_REGION *lock = al_lock_bitmap(_bitmap,
//		ALLEGRO_PIXEL_FORMAT_ARGB_8888, ALLEGRO_LOCK_READONLY);
//
//	unsigned int i, j;
//	for(i = 0; i < size[0]; i++)
//	{
//		for(j = 0; j < size[1]; j++)
//		{
//			//get pixel color
//			ALLEGRO_COLOR col = al_get_pixel(_bitmap, i, j);
//			if(col.a < 0.2)
//			{
//				bitmask_setbit(bitMask, i, j);
//			}
//		}
//	}
//
//	//remember to unlock the bitmap
//	al_unlock_bitmap(_bitmap);
//}

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

	size[0] = (double)al_get_bitmap_width(_bitmap);
	size[1] = (double)al_get_bitmap_height(_bitmap);

	sqrradius = sqrlen(size);

	bounding_box.x = bounding_box.y = 0;
	bounding_box.width = size[0];
	bounding_box.height = size[1];

	//bitMask = bitmask_create(size[0], size[1]);
}

void GameObject::DestroyBitmap()
{
	if(_bitmap != NULL)
	{
		if(!al_is_sub_bitmap(_bitmap))
		{
			LOG_WRITE("Destroying bitmap: %s.", _filename);
			al_destroy_bitmap(_bitmap);
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

	//copy rects and translate them
	Rect a = this->bounding_box;
	Rect b = otherobj->bounding_box;
	b.x -= offset[0];
	b.y -= offset[1];

	if((fabs(a.x - b.x) < (a.width + b.width) / 2) &&
		(fabs(a.y - b.y) < (a.height + b.height) / 2))
	{
		double x_overlap = ((a.width + b.width) / 2) - fabs(a.x - b.x);
		double y_overlap = ((a.height + b.height) / 2) - fabs(a.y - b.y);
		if(x_overlap < y_overlap)
		{
			//determine if we're on left or right
			if((b.x - a.x) > 0)
			{
				//moving left moves away
				this->position[0] += x_overlap;
			}
			else
			{
				this->position[0] -= x_overlap;
			}
			
		}
		else //y overlap is lesser of the two
		{
			//determine if we're on top or bottom
			if((b.y - a.y) > 0)
			{
				//moving up moves away
				this->position[1] += y_overlap;
			}
			else
			{
				this->position[1] -= y_overlap;
			}
			
		}
	
		//LOG_WRITE("Collision at (%f, %f) - overlap is (%f, %f)", offset[0], offset[1], x_overlap, y_overlap);
	}

	//Vec2 offset(otherobj->position - this->position);
	//int xoff = round(offset[0]);
	//int yoff = round(offset[1]);
	//int x, y;
	//if(bitmask_overlap_pos(this->bitMask, otherobj->bitMask, xoff, yoff, &x, &y))
	//{
	//	int dx = bitmask_overlap_area(this->bitMask,otherobj->bitMask,xoff+1,yoff) - 
	//		bitmask_overlap_area(this->bitMask,otherobj->bitMask,xoff-1,yoff);
	//	int dy = bitmask_overlap_area(this->bitMask,otherobj->bitMask,xoff,yoff+1) - 
	//		bitmask_overlap_area(this->bitMask,otherobj->bitMask,xoff,yoff-1);


	//	int offset_mul = 2;

	//	if(dx != 0  || dy != 0)
	//	{
	//		while(bitmask_overlap(this->bitMask, otherobj->bitMask, xoff + (dx * offset_mul), yoff + (dy * offset_mul)))
	//			offset_mul ++;

	//		this->position = this->position + Vec2(dx, dy) * offset_mul;
	//	}

	////	LOG_WRITE("Hey we had a cool collision at (%d,%d) pointing (%d,%d)", x, y, dx, dy);
	//}
}