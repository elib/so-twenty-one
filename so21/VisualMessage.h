#pragma once

#include "svl/svl.h"
#include <allegro5/allegro.h>
#include "GameObject.h"
//#include <vector>
//using namespace std;

enum VISUAL_MESSAGES
{
	MSG_KEYHELP = 0,



	__MSG_LENGTH
};

typedef struct __MSG_INFO
{
	const char filename[1024];
	Vec2 offset;

} MessageInfo;

class VisualMessage
{
public:
	VisualMessage(void);
	~VisualMessage(void);
	
	void Initialize();
	void Update(double delta_time);

	void UpdatePosition(Vec2 absolute_pos);


protected:
	static MessageInfo _message_files[];
	GameObject *_options[__MSG_LENGTH];
	//ALLEGRO_BITMAP* _bitmaps[__MSG_LENGTH];
	VISUAL_MESSAGES _msgDisplayed;
};
