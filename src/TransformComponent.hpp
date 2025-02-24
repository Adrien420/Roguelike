#include "Entity.hpp"

class Vector2D 
{
public:
    float x = 0;
    float y = 0;

    Vector2D() = default;
    Vector2D(float x_, float y_) : x(x_), y(y_) {}
};

class TransformComponent : public Component
{
public:
	Vector2D position;
    Vector2D direction; 

	int height = 64;
	int width = 64;
	int scale = 1;

	float speed = 0.05;

	bool blocked = false;

	TransformComponent(){}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void update() override
	{
		position.x = position.x + direction.x*speed;
		position.y = position.y + direction.y*speed;
	}

};