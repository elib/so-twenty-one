#include "GameObject.h"
#include "World.h"
#include <allegro5\allegro.h>

#ifdef BOUNDINGBOX_ALLOW
#include <allegro5\allegro_primitives.h>
#endif

GameObject::GameObject(double x, double y)
{
	//no-bitmap mode
	_bitmap = NULL;
	_has_bitmap = false;
	strcpy_s(_filename, "[NO BITMAP]");
	position = Vec2(x, y);
	_collidingBody = NULL;
	memset(prettyName, 0, sizeof(prettyName));
	_is_static = true;
}

GameObject::GameObject(const char* filename, double x, double y)
{
	_bitmap = NULL;
	_has_bitmap = true;
	strcpy_s(_filename, filename);
	position = Vec2(x, y);
	_collidingBody = NULL;
	memset(prettyName, 0, sizeof(prettyName));
	_is_static = true;
}

GameObject::GameObject(ALLEGRO_BITMAP *bitmap, double x, double y)
{
	position = Vec2(x, y);
	_has_bitmap = true;
	strcpy_s(_filename, "[MEMORY]");
	_bitmap = bitmap;
	_collidingBody = NULL;
	memset(prettyName, 0, sizeof(prettyName));
	_is_static = true;
}



void GameObject::Update(double delta_time)
{
	_leftscreen = false;
	_frame_rel_pos = Vec2(0.0,0.0);

	if(!_is_static)
	{
		CalculatePhysics(delta_time);
		_collidingBody->SetTransform(World::theWorld->ConvertPointToBox2D(position), 0);
	}

	alpha = MAX(alpha, 0.0);
	alpha = MIN(alpha, 1.0);

	//find relative position to viewport
	_frame_rel_pos = World::theWorld->TranslateToScreen(position, scrollFactor);
	if(_frame_rel_pos[0] + size[0] < 0)
	{
		//we have disappeared off the screen to the left
		_leftscreen = true;
		return;
	}

	if(_frame_rel_pos[1] + size[1] < 0)
	{
		//we have disappeared to the top
		_leftscreen = true;
		return;
	}

	if(_frame_rel_pos[1] + size[1] < 0)
	{
		//we have disappeared to the top
		_leftscreen = true;
		return;
	}

	if(_frame_rel_pos[1] > DISPLAY_HEIGHT)
	{
		//we have disappeared to the bottom
		_leftscreen = true;
		return;
	}

	//more "easy outs" before rendering
	if(_frame_rel_pos[0] > DISPLAY_WIDTH)
		return;

	if(_frame_rel_pos[1] + size[1] < 0)
		return;

	if(_frame_rel_pos[1] > DISPLAY_HEIGHT)
		return;

	//blit
	if(!_has_bitmap && !((bounding_box_debug && World::theWorld->show_boundingbox)))
		return;

	if(_bitmap != NULL)
	{
		if(visible)
		{

			ALLEGRO_BITMAP *the_bitmap;

			//render animation or not
			if(_is_animated)
			{
				the_bitmap = GetCurrentAnimationFrame();
			}
			else
			{
				the_bitmap = _bitmap;
			}

			//don't draw if alpha is very small
			if(alpha > EPSILON)
			{
				//if alpha is about 1, draw regularly
				if(alpha > (1.0 - EPSILON)) //not sure if this does what I think it does
				{
					//al_draw_rotated_scaled_bitmap(the_bitmap, 0, 0, rel_pos[0], rel_pos[1], 1, 1, 0, 0);
					al_draw_bitmap(the_bitmap, _frame_rel_pos[0], _frame_rel_pos[1], 0);
				}
				else
				{
					al_draw_tinted_bitmap(the_bitmap, al_map_rgba_f(1.0, 1.0, 1.0, alpha),
						_frame_rel_pos[0], _frame_rel_pos[1], 0);
				}
			}
		}
	}

	#ifdef BOUNDINGBOX_ALLOW
	if(bounding_box_debug && World::theWorld->show_boundingbox)
	{
		//new: draw box2d shape, or something
		b2Vec2 pos = _collidingBody->GetPosition();
		Vec2 bound_origin = World::theWorld->ConvertBox2DToScreen(pos);
		//Vec2 bound_origin = Vec2(_frame_rel_pos[0] + bounding_box.x, _frame_rel_pos[1] + bounding_box.y);
		al_draw_rectangle(bound_origin[0], bound_origin[1], bound_origin[0] + bounding_box.width, bound_origin[1] + bounding_box.height,
				al_map_rgba_f(1.0, 0, 0, 0.5), 0);
	}
	#endif
}

void GameObject::CalculatePhysics(double delta_time)
{
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

	//END PHYSICS
}

void GameObject::Initialize()
{
	//set is_animated to false - you have to change that some other way.
	_is_animated = false;
	_animation_time = 0;

	//default: show bounding box
	bounding_box_debug = true;

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
		size[0] = size[1] = 32.0;
	}

	sqrradius = sqrlen(size);

	bounding_box.x = bounding_box.y = 0;
	bounding_box.width = size[0];
	bounding_box.height = size[1];

	InitializeCollisions();


	strcpy_s(prettyName, "DUNNO");

}

void GameObject::InitializeCollisions()
{
	b2BodyDef def = GetBodyDef();
	
	def.position = World::theWorld->ConvertPointToBox2D(position);
	_collidingBody = World::theWorld->box2dWorld->CreateBody(&def);
	SetFixtureDef();
}

void GameObject::SetCollisionCategory(unsigned short category)
{
	b2Filter filter;
	filter.categoryBits = category;
	filter.maskBits = category;
	//_collidingBody->GetFixtureList()[0].SetFilterData(filter);
}

void GameObject::SetFixtureDef()
{
	b2Vec2 box_dimensions = World::theWorld->ConvertPointToBox2D(bounding_box.width, bounding_box.height);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(box_dimensions.x, box_dimensions.y, b2Vec2(0,0), 0);

	b2FixtureDef fixture;
	fixture.shape = &dynamicBox;
	fixture.density = 1.0f;
	fixture.friction = 0.3f;
	_collidingBody->CreateFixture(&fixture);
}

b2BodyDef GameObject::GetBodyDef()
{
	b2BodyDef def;
	def.type = b2_staticBody;
	def.active = true;
	def.awake = true;
	def.userData = this;
	return def;
}

void GameObject::OnCollide(GameObject *otherobj, b2Contact *contact)
{
	//do nothing by default.
	return;
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
}

bool GameObject::LeftScreen()
{
	return _leftscreen;
}

bool GameObject::Collide(GameObject *otherobj)
{

	//copy rects for comfort
	Rect a = this->bounding_box;
	Rect b = otherobj->bounding_box;

	//move to center of bounding boxes
	Vec2 bounding_a = this->position + Vec2(a.x, a.y) + Vec2(a.width/2, a.height/2);
	Vec2 bounding_b = otherobj->position + Vec2(b.x, b.y) + Vec2(b.width/2, b.height/2);
	
	Vec2 offset(bounding_b - bounding_a);

	//check proximity
	if(sqrlen(offset) >  1.2 * MAX(this->sqrradius, otherobj->sqrradius))
		return false;

	double totalwidth = (a.width + b.width)/2;
	double totalheight = (a.height + b.height)/2;

	Vec2 absdiff = Vec2(fabs(offset[0]), fabs(offset[1]));

	if((absdiff[0] < totalwidth) &&
		(absdiff[1] < totalheight))
	{
		double x_overlap = totalwidth - absdiff[0];
		double y_overlap = totalheight - absdiff[1];

		Vec2 correction = Vec2(0.0, 0.0);

		if(x_overlap < y_overlap)
		{
			//determine if we're on left or right
			if((offset[0]) < 0)
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
			if((offset[1]) < 0)
			{
				//moving up moves away
				correction[1] = + y_overlap;
			}
			else
			{
				correction[1] = - y_overlap;
			}
		}

		if(sqrlen(correction) > 0)
		{
			this->position += correction;
			return true;
		}
		
	}

	return false;
}

void GameObject::InitializeAnimation(int frame_width, int fps)
{
	//bitmap should already be loaded.
	unsigned int frame_count = size[0] / frame_width;
	unsigned int i;
	for(i = 0; i < frame_count; i++)
	{
		ALLEGRO_BITMAP *sub_bitmap = al_create_sub_bitmap(_bitmap, i * frame_width, 0, frame_width, size[1]);
		_animation_frames.push_back(sub_bitmap);
	}

	_animation_time = 1/((double) fps);
	_is_animated = true;
}

ALLEGRO_BITMAP* GameObject::GetCurrentAnimationFrame()
{
	unsigned int frame_index = 
		((int)(World::theWorld->total_time / _animation_time)) % _animation_frames.size();
	return _animation_frames[frame_index];
}