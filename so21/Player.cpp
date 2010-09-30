#include "Player.h"

#include "World.h"

void Player::Initialize()
{
	GameObject::Initialize();

	max_velocity = 160;
	damping = 400;

	bounding_box.x = -1;
	bounding_box.y = 7;
	bounding_box.width = 34;
	bounding_box.height = 19;
}


void Player::Update(double delta_time)
{
	bool move_ver = false;
	bool move_hor = false;
	static const double accel = 850;

	//accept input for player sprite
	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_W]
		|| World::theWorld->Keys.keys_down[ALLEGRO_KEY_UP])
	{
		this->acceleration[1] = -1;
		move_ver = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_S]
		|| World::theWorld->Keys.keys_down[ALLEGRO_KEY_DOWN])
	{
		this->acceleration[1] = 1;
		move_ver = true;
	}

	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_A]
		|| World::theWorld->Keys.keys_down[ALLEGRO_KEY_LEFT])
	{
		this->acceleration[0] = -1;
		move_hor = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_D]
		|| World::theWorld->Keys.keys_down[ALLEGRO_KEY_RIGHT])
	{
		this->acceleration[0] = 1;
		move_hor = true;
	}

	if(!move_hor)
	{
		acceleration[0] = 0.0;
	}

	if(!move_ver)
	{
		acceleration[1] = 0.0;
	}

	if(move_hor || move_ver)
	{
		//normalize acceleration
		acceleration = norm(acceleration) * accel;
	}

	GameObject::Update(delta_time);

	if(!_notifiedPlayerLeftSpawn)
	{
		if((position[0] - _starting_location[0]) > 20)
		{
			World::theWorld->PlayerLeftSpawn();
			_notifiedPlayerLeftSpawn = true;
		}
	}

	//now, check we are inside the bounds of the screen

	//X
	if(_frame_rel_pos[0] + bounding_box.x < 0)
		position[0] -= _frame_rel_pos[0] + bounding_box.x;
	if(_frame_rel_pos[0] + bounding_box.x + bounding_box.width > DISPLAY_WIDTH)
		position[0] -= _frame_rel_pos[0] + bounding_box.x + bounding_box.width - DISPLAY_WIDTH;

	//Y
	if(_frame_rel_pos[1] + bounding_box.y < 0)
		position[1] -= _frame_rel_pos[1] + bounding_box.y;
	if(_frame_rel_pos[1] + bounding_box.y + bounding_box.height > DISPLAY_HEIGHT)
		position[1] -= _frame_rel_pos[1] + bounding_box.y + bounding_box.height - DISPLAY_HEIGHT;
}