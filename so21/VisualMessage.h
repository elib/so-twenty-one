#pragma once

#include "svl/svl.h"
#include <allegro5/allegro.h>
#include "GameObject.h"

#include "quickmax.h"



enum VISUAL_MESSAGE
{
	MSG_KEYHELP = 0,

	__MSG_LENGTH
};

enum MSG_STATE
{
	MSG_INVISIBLE,
	MSG_FADEIN,
	MSG_VISIBLE,
	MSG_FADEOUT
};

typedef struct __MSG_INFO
{
	const char filename[1024];
	MSG_STATE state; //current state 
	double start_time; //time left last steady state into transitional state
	double fade_time; //target time for next steady state (if in transitional state)
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

	void FadeInMsg(VISUAL_MESSAGE desired_msg, double fade_time);
	void FadeOutMsg(VISUAL_MESSAGE desired_msg, double fade_time);


protected:
	static MessageInfo _message_files[];
	GameObject *_options[__MSG_LENGTH];
};
