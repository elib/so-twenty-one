#include "Title.h"

Title::~Title()
{
	delete _title1;
	delete _title2;
}

void Title::Initialize()
{
	GameObject::Initialize();

	_title1 = new GameObject("Resources/title1.png", position[0], position[1]);
	_title1->Initialize();
	strcpy_s(_title1->prettyName, "Title1");
	//_title1->alpha = 0;

	_title2 = new GameObject("Resources/title2.png", position[0], position[1]);
	_title2->Initialize();
	strcpy_s(_title2->prettyName, "Title2");
	//_title2->alpha = 0;
}

void Title::Update(double delta_time)
{
	GameObject::Update(delta_time);

	_title1->Update(delta_time);
	_title2->Update(delta_time);
}