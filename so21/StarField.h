#pragma once

#include "GameObject.h"
#include <vector>
using namespace std;

class StarField
{
public:
	StarField(void);
	~StarField(void);

	void Initialize();
	void Destroy();
	void Update(double delta_time);

protected:
	//resources:
	static const char *_starsImageSourceFile;
	static const int _starsize;
	static const int _starcount;

	void LoadAvailableBitmaps();
	
	//objects and their bitmaps
	vector<GameObject*> _starLayer;

	vector<ALLEGRO_BITMAP*> _availableBitmaps;
	ALLEGRO_BITMAP *_largeBitmap;
};