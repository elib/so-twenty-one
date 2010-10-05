#include "VisualMessage.h"
#include "World.h"

MessageInfo VisualMessage::_message_files[] = 
{
	{"Resources/keyhelp.png", Vec2(32, -86)}
};


VisualMessage::VisualMessage(void)
{
	_msgDisplayed = MSG_KEYHELP;
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
	for(i = 0; i < __MSG_LENGTH; i++)
	{
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