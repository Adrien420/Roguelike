#pragma once

#include "GameManager.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "ProjectileComponent.hpp"
#include <map>

class KeyboardController : public Component
{
	private:
		EntitiesManager& entitiesManager = GameManager::entitiesManager;
		StatisticsComponent *stats;
		std::string playerId;
		SDL_KeyCode upKey;
		int attackDuration;
		Uint32 attackStart;
		bool isAttacking = false;
		bool projectileSent = false;
		// Commentaire temporaire : 
		// Si d est pressé (direction.x = 1), puis q est pressé (direction.x = -1) et relâché (direction = 0)
		// Même si d est toujours pressé, direction.x reste à 0
		// Donc l'idée de ce dictionnaire est de vérifier si la touche pour la direction opposée a bien été relâchée avant de remettre la direction à 0
		std::map<const std::string, bool> isBeingPressed = {{"Up",false}, {"Down",false}, {"Left",false}, {"Right",false}};
		std::map<const std::string, int> velocity = {{"Up",-1}, {"Down",1}, {"Left",-1}, {"Right",1}};
		std::map<const std::string, std::map<const std::string, const SDL_KeyCode>> playerKeys = {{"player1", {{"Up",SDLK_z}, {"Down",SDLK_s}, {"Left",SDLK_q}, {"Right",SDLK_d}, {"Attack",SDLK_SPACE}}},
		{"player2", {{"Up",SDLK_UP}, {"Down",SDLK_DOWN}, {"Left",SDLK_LEFT}, {"Right",SDLK_RIGHT}, {"Attack",SDLK_m}}}};

	public:
		TransformComponent *transform;
		SpriteComponent *sprite;
		Entity *projectile;

		KeyboardController(std::string playerId_) : playerId(playerId_) {}

		void init() override
		{
			transform = &entity->getComponent<TransformComponent>();
			stats = &entity->getComponent<StatisticsComponent>();
			sprite = &entity->getComponent<SpriteComponent>();
			attackDuration = sprite->animations["Attack Down"]["frameTime"] * sprite->animations["Attack Down"]["frames"];
		}

		void applyDirection(std::string direction)
		{
			if(direction == "None")
			{
				transform->direction.y = 0;
				transform->direction.x = 0;
				return;
			}

			isBeingPressed[direction] = true;
			if(isAttacking)
				return;
			else if(direction == "Up" || direction == "Down")
			{
				transform->direction.y = velocity[direction];
				transform->direction.x = 0;
			}
			else
			{
				transform->direction.y = 0;
				transform->direction.x = velocity[direction];
			}
			sprite->Play("Walk " + direction);
		}

		void changeDirection(std::string direction, bool keyReleased)
		{
			if(keyReleased)
				isBeingPressed[direction] = false;
			if(isAttacking)
				return;
			for (const auto& pair : isBeingPressed) {
				// Si une autre touche directionnelle est encore pressée : changement de direction (ordre de priorité arbitraire -> déterminé par la map)
				if(pair.second)
				{
					applyDirection(pair.first);
					return;
				}
			}
			// Si aucune autre touche directionnelle n'est pressée
			applyDirection("None");
			sprite->Play("Idle " + direction);
		}

		void attack()
		{
			if(isAttacking)
				return;
			applyDirection("None");
			Vector2D projectileDirection = Vector2D();
			Vector2D projectilePosition = Vector2D(transform->position.x, transform->position.y);
			int directionIndex = sprite->animIndex%4;
			switch(directionIndex)
			{
				case 0:
					sprite->Play("Attack Down");
					projectileDirection.y = 1;
					projectilePosition.x += transform->width/2;
					projectilePosition.y += transform->height;
					break;
				case 1:
					sprite->Play("Attack Up");
					projectileDirection.y = -1;
					projectilePosition.x += transform->width/2;
					break;
				case 2:
					sprite->Play("Attack Left");
					projectileDirection.x = -1;
					projectilePosition.y += transform->height/2;
					break;
				case 3:
					sprite->Play("Attack Right");
					projectileDirection.x = 1;
					projectilePosition.x += transform->width;
					projectilePosition.y += transform->height/2;
					break;
				default:
					break;
			}
			isAttacking = true;
			attackStart = SDL_GetTicks();
			if(stats->hasProjectiles && (!projectileSent))
			{
				projectile = new Entity(TransformComponent(projectilePosition.x, projectilePosition.y,64,64,0.75), StatisticsComponent(800, 100, 0.12, 100), ProjectileComponent(projectileDirection));
				entitiesManager.addEntity(projectile);
				projectileSent = true;
			}
		}

		void update() override
		{
			if(isAttacking && (SDL_GetTicks() - attackStart > attackDuration))
			{
				isAttacking = false;
				projectileSent = false;
				int directionIndex = sprite->animIndex%4;
				switch(directionIndex)
				{
					case 0:
						changeDirection("Down", false);
						break;
					case 1:
						changeDirection("Up", false);
						break;
					case 2:
						changeDirection("Left", false);
						break;
					case 3:
						changeDirection("Right", false);
						break;
					default:
						break;
				}
			}
			if (GameManager::event.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = GameManager::event.key.keysym.sym;
				switch (key)
				{
					case SDLK_z: case SDLK_UP:
						if(playerKeys[playerId]["Up"] == key)
							applyDirection("Up");
						break;
					case SDLK_q: case SDLK_LEFT:
						if(playerKeys[playerId]["Left"] == key)
							applyDirection("Left");
						break;
					case SDLK_d: case SDLK_RIGHT:
						if(playerKeys[playerId]["Right"] == key)
							applyDirection("Right");
						break;
					case SDLK_s: case SDLK_DOWN:
						if(playerKeys[playerId]["Down"] == key)
							applyDirection("Down");
						break;
					case SDLK_SPACE: case SDLK_m:
						if(playerKeys[playerId]["Attack"] == key)
							attack();
						break;
					default:
						break;
				}
			}
		
			if (GameManager::event.type == SDL_KEYUP)
			{
				SDL_Keycode key = GameManager::event.key.keysym.sym;
				switch (key)
				{
					case SDLK_z: case SDLK_UP:
						if(playerKeys[playerId]["Up"] == key)
							changeDirection("Up", true);
						break;

					case SDLK_q: case SDLK_LEFT:
						if(playerKeys[playerId]["Left"] == key)
							changeDirection("Left", true);
						break;

					case SDLK_d: case SDLK_RIGHT:
						if(playerKeys[playerId]["Right"] == key)
							changeDirection("Right", true);
						break;

					case SDLK_s: case SDLK_DOWN:
						if(playerKeys[playerId]["Down"] == key)
							changeDirection("Down", true);
						break;

					case SDLK_ESCAPE:
						GameManager::isRunning = false;
					default:
						break;
				}
			}
		}

		void reset() override
		{
			transform->direction.x = 0;
			transform->direction.y = 0;
			sprite->Play("Idle Down");
		}
};