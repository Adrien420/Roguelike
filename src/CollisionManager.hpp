#pragma once
#include <SDL2/SDL.h>

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};