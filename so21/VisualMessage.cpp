#include "VisualMessage.h"
#include "World.h"

MessageInfo VisualMessage::_message_files[] = 
{
	{"Resources/keyhelp.png", MSG_INVISIBLE, 0.0, 0.0, Vec2(32, -86)}
};


VisualMessage::VisualMessage(void)
{
}

VisualMessage::~VisualMessage(void)
{
	unsigned int i;
	for(i = 0; i < __MSG_LENGTH; i++)
	{
		delete _options[i];
	}
}


void VisualMessage::Initialize()
{
	unsigned int i;
	for(i = 0; i < __MSG_LENGTH; i++)
	{
		_options[i] = new GameObject(_message_files[i].filename, 0, 0);
		_options[i]->Initialize();
		//_options[i]->visible = false;
	}
}

void VisualMessage::Update(double delta_time)
{
	unsigned int i;
	double total_time = World::theWorld->total_time;

	for(i = 0; i < __MSG_LENGTH; i++)
	{
		if(_message_files[i].state == MSG_FADEIN)
		{
			if(total_time >= _message_files[i].fade_time)
				_options[i]->alpha = 1.0;
			else
			{
				_options[i]->alpha =
					ease_in(
					(total_time - _message_files[i].start_time) /
						(_message_files[i].fade_time - _message_files[i].start_time));
			}
		}
		else if (_message_files[i].state == MSG_FADEOUT)
		{
			if(total_time >= _message_files[i].fade_time)
			{
				_options[i]->alpha = 0.0;
				_message_files[i].state = MSG_INVISIBLE;
			}
			else
			{
				_options[i]->alpha =
					ease_out(
					(total_time - _message_files[i].start_time) /
						(_message_files[i].fade_time - _message_files[i].start_time));
			}
		}
		
		//and in any case besides invisibility
		if(_message_files[i].state != MSG_INVISIBLE)
			_options[i]->Update(delta_time);
	}
}

void VisualMessage::UpdatePosition(Vec2 absolute_pos)
{
	unsigned int i;
	for(i = 0; i < __MSG_LENGTH; i++)
	{
		_options[i]->position = absolute_pos + _message_files[i].offset;
	}
}

void VisualMessage::FadeInMsg(VISUAL_MESSAGE desired_msg, double fade_time)
{
	if((_message_files[desired_msg]).state == MSG_FADEIN
		|| (_message_files[desired_msg]).state == MSG_VISIBLE)
		return;

	_message_files[desired_msg].state = MSG_FADEIN;
	_message_files[desired_msg].fade_time = World::theWorld->total_time + fade_time;
	_message_files[desired_msg].start_time = World::theWorld->total_time;
}

void VisualMessage::FadeOutMsg(VISUAL_MESSAGE desired_msg, double fade_time)
{
	if((_message_files[desired_msg]).state == MSG_FADEOUT
		|| (_message_files[desired_msg]).state == MSG_INVISIBLE)
		return;

	_message_files[desired_msg].state = MSG_FADEOUT;
	_message_files[desired_msg].fade_time = World::theWorld->total_time + fade_time;
	_message_files[desired_msg].start_time = World::theWorld->total_time;
}