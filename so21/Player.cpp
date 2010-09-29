#include "Player.h"

#include "World.h"

void Player::Initialize()
{
	GameObject::Initialize();

	max_velocity = 160;
	damping = 400;

	//make alpha bitmask!
	//GenerateMaskByAlpha();
}


void Player::Update(double delta_time)
{
	bool move_ver = false;
	bool move_hor = false;
	static const double accel = 850;

	//accept input for player sprite
	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_W])
	{
		this->acceleration[1] = -1;
		move_ver = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_S])
	{
		this->acceleration[1] = 1;
		move_ver = true;
	}

	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_A])
	{
		this->acceleration[0] = -1;
		move_hor = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_D])
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
}