#include "Entity.hpp"
#include "StatisticsComponent.hpp"

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
	private:
		StatisticsComponent *stats;

	public:
		Vector2D position;
		Vector2D direction; 

		int height = 64;
		int width = 64;
		float scale = 1.0;

		float speed = 0.07;

		bool blocked = false;

		TransformComponent(){}

		TransformComponent(float x, float y, int h, int w, float sc)
		{
			position.x = x;
			position.y = y;
			height = h;
			width = w;
			scale = sc;
		}

		void init() override
		{
			stats = &entity->getComponent<StatisticsComponent>();
			speed = std::get<float>(stats->stats["speed"]);
		}

		void update() override
		{
			if(GameManager::inDeathAnimation)
				return;
				
			position.x = position.x + direction.x*speed;
			position.y = position.y + direction.y*speed;
		}

		void reset() override
		{
			speed = std::get<float>(stats->stats["speed"]);
		}
};