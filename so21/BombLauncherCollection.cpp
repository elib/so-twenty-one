#include "BombLauncherCollection.h"
#include "World.h"

#define BOMBLAUNCHER_FLASHINGBOMBS_1		0
#define BOMBLAUNCHER_LASERTOPBOTTOM_1		1
#define BOMBLAUNCHER_LASERTOPBOTTOM_2		2

void BombLauncherCollection::Inititalize()
{
	//BOMBLAUNCHER_FLASHINGBOMBS_1
	BombLauncher *launcher = new BombLauncher(LAUNCHABLE_FLASHING_BOMB, Vec2(DISPLAY_WIDTH, DISPLAY_HEIGHT/2 - 16));
	launcher->Initialize();
	_launchers.push_back(launcher);

	//BOMBLAUNCHER_LASERTOPBOTTOM_1
	launcher = new BombLauncher(LAUNCHABLE_LASER, Vec2(DISPLAY_WIDTH, 1*DISPLAY_HEIGHT/10 - 16));
	launcher->Initialize();
	_launchers.push_back(launcher);

	//BOMBLAUNCHER_LASERTOPBOTTOM_2
	launcher = new BombLauncher(LAUNCHABLE_LASER, Vec2(DISPLAY_WIDTH, 9*DISPLAY_HEIGHT/10 - 16));
	launcher->Initialize();
	_launchers.push_back(launcher);

}

void BombLauncherCollection::Update(double delta)
{
	unsigned int i;
	for(i = 0; i < _launchers.size(); i++)
	{
		_launchers[i]->Update(delta);
	}
}

void BombLauncherCollection::ConsumeEvent(MusicEvent one_event)
{
	unsigned int type = one_event.type;

	switch(type)
	{
	case 2:
		_launchers[BOMBLAUNCHER_FLASHINGBOMBS_1]->LaunchBomb();
		break;
	case 3:
		_launchers[BOMBLAUNCHER_LASERTOPBOTTOM_1]->LaunchBomb();
		_launchers[BOMBLAUNCHER_LASERTOPBOTTOM_2]->LaunchBomb();
		break;
	}

}

void BombLauncherCollection::Collide(GameObject *other)
{
	unsigned int i;
	for(i = 0; i < _launchers.size(); i++)
	{
		_launchers[i]->Collide(other);
	}
}