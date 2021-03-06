#include "SpawnPoint.h"
#include "World.h"


void SpawnPoint::Initialize()
{
	GameObject::Initialize();

	this->bounding_box_debug = false;

	//top
	GameObject *obj = new GameObject(position[0], position[1]);
	obj->Initialize();
	obj->bounding_box.height = 2;
	obj->bounding_box.y = 9;
	obj->SetCollisionCategory(PLAYER_COLLISION_CATEGORY);
	strcpy_s(obj->prettyName, "Spawnpoint-1");
	_sub_colliders.push_back(obj);


	//left
	obj = new GameObject(position[0], position[1]);
	obj->Initialize();
	obj->bounding_box.width = 2;
	obj->bounding_box.x = 0;
	obj->bounding_box.y = 10;
	obj->SetCollisionCategory(PLAYER_COLLISION_CATEGORY);
	strcpy_s(obj->prettyName, "Spawnpoint-2");
	_sub_colliders.push_back(obj);

	//bottom
	obj = new GameObject(position[0], position[1]);
	obj->Initialize();
	obj->bounding_box.height = 2;
	obj->bounding_box.y = 25;
	obj->SetCollisionCategory(PLAYER_COLLISION_CATEGORY);
	strcpy_s(obj->prettyName, "Spawnpoint-3");
	_sub_colliders.push_back(obj);

}

SpawnPoint::~SpawnPoint()
{
	unsigned int i;
	for(i = 0; i < _sub_colliders.size(); i++)
	{
		delete _sub_colliders[i];
	}
}

void SpawnPoint::Update(double delta_time)
{
	GameObject::Update(delta_time);

	unsigned int i;
	for(i = 0; i < _sub_colliders.size(); i++)
	{
		_sub_colliders[i]->Update(delta_time);
	}
}

bool SpawnPoint::Collide(GameObject *otherobj)
{
	//does NOT call parent
	//(otherwise we would collide with this object, which is the spawn container)

	unsigned int i;
	bool collision = false;
	for(i = 0; i < _sub_colliders.size(); i++)
	{
		collision |= otherobj->Collide(_sub_colliders[i]);
	}

	return collision;

}