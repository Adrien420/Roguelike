#include "Entity.hpp"
#include "StatisticsComponent.hpp"

class Vector2D 
{
public:
    float x = 0;
    float y = 0;

    Vector2D() = default;
    Vector2D(float x_, float y_) : x(x_), y(y_) {}

	Vector2D rotate(float angleDegrees) const 
	{
		float angleRadians = angleDegrees * M_PI / 180.0f; // Conversion en radians
		float cosA = cos(angleRadians);
		float sinA = sin(angleRadians);
		return Vector2D(x * cosA - y * sinA, x * sinA + y * cosA);
	}
};

class TransformComponent : public Component
{
	private:
		StatisticsComponent *stats;
		bool useCustomSpeed = false; // Permet de savoir si on utilise le speed des stats

	public:
		Vector2D position;
		Vector2D direction; 

		int height = 64;
		int width = 64;
		float scale = 1.0;

		float speed = 0.07;

		bool blocked = false;

		TransformComponent() {}

		// Constructeur qui utilise les stats pour la vitesse
		TransformComponent(float x, float y, int h, int w, float sc)
			: position(x, y), height(h), width(w), scale(sc) {}

		// Nouveau constructeur qui permet de définir une vitesse spécifique
		TransformComponent(float x, float y, int h, int w, float sc, float customSpeed)
			: position(x, y), height(h), width(w), scale(sc), speed(customSpeed), useCustomSpeed(true) {}

		void init() override
		{
			// Associe le speed aux stats uniquement si on n'a pas défini une vitesse custom
			if (!useCustomSpeed) 
			{
				stats = &entity->getComponent<StatisticsComponent>();
				speed = std::get<float>(stats->stats["speed"]);
			}
		}

		void update() override
		{
			if (GameManager::inDeathAnimation)
				return;

			position.x += direction.x * speed;
			position.y += direction.y * speed;
		}

		void reset() override
		{
			if (!useCustomSpeed) 
			{
				speed = std::get<float>(stats->stats["speed"]);
			}
			std::string playerId = entity->playerId;
			if (playerId == "player1"){position.x = 0;}
			if (playerId == "player2"){position.x = 1280 - 100;}
			position.y = 368;
		}
};
