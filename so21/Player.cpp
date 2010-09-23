#include "Player.h"

#include "World.h"


void Player::Update(double delta_time)
{
	bool move_ver = false;
	bool move_hor = false;
	double speed = 15;

	//accept input for player sprite
	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_W])
	{
		this->velocity[1] = -speed;
		move_ver = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_S])
	{
		this->velocity[1] = speed;
		move_ver = true;
	}

	if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_A])
	{
		this->velocity[0] = -speed;
		move_hor = true;
	}
	else if(World::theWorld->Keys.keys_down[ALLEGRO_KEY_D])
	{
		this->velocity[0] = speed;
		move_hor = true;
	}

	if(!move_hor)
	{
		velocity[0] = 0.0;
	}

	if(!move_ver)
	{
		velocity[1] = 0.0;
	}

	GameObject::Update(delta_time);
}