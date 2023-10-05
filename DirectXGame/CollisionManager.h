#pragma once
#include "Collider.h"

#include <list>

class CollisionManager 
{
public:
	std::list<Collider*> colliders_;

public:
	void CheckAllCollision();

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

public:
	CollisionManager(){};
	~CollisionManager() = default;

};
