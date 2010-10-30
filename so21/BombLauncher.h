#pragma once

#include "GameObject.h"
#include "FlashingBomb.h"

#include <vector>
using namespace std;

#define NUM_BOMBS	20

class BombLauncher
{
public:
	BombLauncher(void);
	~BombLauncher(void);

	void Initialize();
	void Update(double delta_time);
	//void Collide(

	void SpawnBomb(double X, double Y);
	

protected:
	vector<FlashingBomb*> _bombs_in_play;
	vector<FlashingBomb*> _bombs_available;

private:
	void RemoveAllBombs();
};