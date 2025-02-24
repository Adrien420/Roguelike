#include "Entity.hpp"

class Vector2 
{
    private:
        float x = 0;
        float y = 0;

    public:
        float getX() const { return x; }
        float getY() const { return y; }

        void setX(float x_) { x = x_; }
        void setY(float y_) { y = y_; }
};

class TransformComponent : public Component
{
public:
	Vector2 position;
    Vector2 direction; 

	int height = 64;
	int width = 64;
	int scale = 1;

	float speed = 0.02;

	bool blocked = false;

	TransformComponent(){}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.setX(x);
		position.setY(y);
		height = h;
		width = w;
		scale = sc;
	}

	void update() override
	{
		position.setX(position.getX()+direction.getX()*speed);
		position.setY(position.getY()+direction.getY()*speed);
	}

};