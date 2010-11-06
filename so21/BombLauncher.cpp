#include "BombLauncher.h"
#include "World.h"

BombLauncher::BombLauncher(void)
{
	//yay for perfect placements
	offset_position = Vec2(DISPLAY_WIDTH, DISPLAY_HEIGHT/2 - 16);
	_speed = 80;
}

void BombLauncher::Initialize()
{
	unsigned int i;
	for(i = 0; i < NUM_BOMBS; i++)
	{
		FlashingBomb* b = new FlashingBomb(0,0);
		b->Initialize();
		_bombs_available.push_back(b);
	}
}

void BombLauncher::Update(double delta_time)
{
	//update position in relation to the camera
	_position = World::theWorld->cameraPosition + offset_position;

	int i;
	for ( i = (_bombs_in_play.size() - 1); i >= 0; i-- )
	{
		_bombs_in_play[i]->Update(delta_time);
		
		if(_bombs_in_play[i]->LeftScreen())
		{
			_bombs_available.push_back(_bombs_in_play[i]);
			_bombs_in_play.erase(_bombs_in_play.begin() + i);
		}
	}
}

void BombLauncher::Collide(GameObject *otherobj)
{

}

void BombLauncher::LaunchBomb()
{
	if(_bombs_available.size() > 0)
	{
		//we can launch one - it's available
		BombListIterator it = _bombs_available.begin();
		FlashingBomb* bomb = _bombs_available[0];
		bomb->position = _position;
		bomb->velocity = _speed * ((World::theWorld->PlayerPosition() - _position).Normalise());
		_bombs_in_play.push_back(bomb);
		_bombs_available.erase(it);
	}
}

BombLauncher::~BombLauncher(void)
{
	RemoveAllBombs();
}

void BombLauncher::ClearScreenBombs()
{
	//move everything to the "available" list

	unsigned int i;
	for(i = 0; i < _bombs_in_play.size(); i++)
	{
		_bombs_available.push_back(_bombs_in_play[i]);
	}

	_bombs_in_play.clear();
}

void BombLauncher::RemoveAllBombs()
{
	ClearScreenBombs();

	//the in-play list should now be empty.

	unsigned int i;
	for(i = 0; i < _bombs_available.size(); i++)
		delete (_bombs_available.at(i));

	_bombs_available.clear();
}