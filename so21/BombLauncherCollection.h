#pragma once

#include "MusicProvider.h"
#include "BombLauncher.h"
#include "GameObject.h"
#include <vector>
using namespace std;

class BombLauncherCollection
{
public:
	void Inititalize();
	void Update(double delta);
	void Collide(GameObject *other);
	void ConsumeEvent(MusicEvent one_event);

private:
	vector<BombLauncher*> _launchers;
};