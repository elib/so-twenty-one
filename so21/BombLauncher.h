#pragma once

#include "GameObject.h"
#include "FlashingBomb.h"

#include <vector>
using namespace std;

#define NUM_BOMBS	30

typedef vector<FlashingBomb*> BombList;
typedef BombList::iterator BombListIterator;
typedef BombList::reverse_iterator BombListReverseIterator;

class BombLauncher
{
public:
	BombLauncher(void);
	~BombLauncher(void);

	void Initialize();
	void Update(double delta_time);
	void ClearScreenBombs();

	void LaunchBomb();

	void Collide(GameObject *otherobj);

	Vec2 offset_position;

protected:
	BombList _bombs_in_play;
	BombList _bombs_available;

private:
	void RemoveAllBombs();
	Vec2 _position;
};