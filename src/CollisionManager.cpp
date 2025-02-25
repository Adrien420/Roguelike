#include "CollisionManager.hpp"
#include "ColliderComponent.hpp"

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	const auto& recA = colA.collider;
	const auto& recB = colB.collider;

	return recA.x + recA.w >= recB.x &&
	       recB.x + recB.w >= recA.x &&
	       recA.y + recA.h >= recB.y &&
	       recB.y + recB.h >= recA.y;
}