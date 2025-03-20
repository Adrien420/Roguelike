#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

#include "GameManager.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <map>

class KeyboardController : public Component
{
	protected:
		EntitiesManager& entitiesManager = GameManager::entitiesManager;
		StatisticsComponent *stats;
		std::string playerId;
		SDL_Keycode upKey;
		int attackDuration;
		Uint32 attackStart;
		bool isAttacking = false;
		// isSpamming : Empêche de pouvoir attaquer en continu en maintenant la touche d'attaque 
		// => Permet d'éviter de devoir utiliser des threads
		bool isSpamming = false; 
		bool projectileSent = false;
		// Commentaire temporaire : 
		// Si d est pressé (direction.x = 1), puis q est pressé (direction.x = -1) et relâché (direction = 0)
		// Même si d est toujours pressé, direction.x reste à 0
		// Donc l'idée de ce dictionnaire est de vérifier si la touche pour la direction opposée a bien été relâchée avant de remettre la direction à 0
		std::map<const std::string, bool> isBeingPressed = {{"Up",false}, {"Down",false}, {"Left",false}, {"Right",false}};
		std::map<const std::string, int> velocity = {{"Up",-1}, {"Down",1}, {"Left",-1}, {"Right",1}};
		std::map<const std::string, std::map<const std::string, const SDL_Keycode>> playerKeys = {{"player1", {{"Up",SDLK_z}, {"Down",SDLK_s}, {"Left",SDLK_q}, {"Right",SDLK_d}, {"Attack",SDLK_SPACE}}},
		{"player2", {{"Up",SDLK_UP}, {"Down",SDLK_DOWN}, {"Left",SDLK_LEFT}, {"Right",SDLK_RIGHT}, {"Attack",SDLK_m}}}};

	public:
		TransformComponent *transform;
		SDL_Rect *collider;
		SpriteComponent *sprite;
		Entity *projectile, *sword;

		KeyboardController(std::string playerId_) : playerId(playerId_) {}

		void init() override
		{
			transform = &entity->getComponent<TransformComponent>();
			collider = &entity->getComponent<ColliderComponent>().collider;
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
			Vector2D attackDirection = Vector2D();
			Vector2D attackPosition = Vector2D(collider->x, collider->y);
			int p_offsetX = 0, p_offsetY = 0;
			int s_offsetX = 0, s_offsetY = 0, s_offsetW = 0, s_offsetH = 0;
			int directionIndex = sprite->animIndex % 4;

			switch (directionIndex)
			{
				case 0: // Bas
					sprite->Play("Attack Down");
					attackDirection.y = 1;
					attackPosition.x += collider->w / 2;
					attackPosition.y += collider->h;
					p_offsetX = -32;
					s_offsetX = -96;
					s_offsetW = 128;
					break;
				case 1: // Haut
					sprite->Play("Attack Up");
					attackDirection.y = -1;
					attackPosition.x += collider->w / 2;
					p_offsetX = -32;
					p_offsetY = 64;
					s_offsetX = -96;
					s_offsetY = -64;
					s_offsetW = 128;
					break;
				case 2: // Gauche
					sprite->Play("Attack Left");
					attackDirection.x = -1;
					attackPosition.y += collider->h / 2;
					p_offsetX = -64;
					p_offsetY = -32;
					s_offsetX = -64;
					s_offsetY = -96;
					s_offsetH = 128;
					break;
				case 3: // Droite
					sprite->Play("Attack Right");
					attackDirection.x = 1;
					attackPosition.x += collider->w;
					attackPosition.y += collider->h / 2;
					p_offsetY = -32;
					s_offsetY = -96;
					s_offsetH = 128;
					break;
				default:
					break;
			}

			p_offsetX *= 0.75;
			p_offsetY *= 0.75;
			isAttacking = true;
			attackStart = SDL_GetTicks();

			// Création de l'épée
			sword = new Entity(StatisticsComponent(0, 0, 0, 0, 0), TransformComponent(attackPosition.x, attackPosition.y, 0, 0, 0.75), ColliderComponent("sword", s_offsetX, s_offsetY, 64 + s_offsetW, 64 + s_offsetH));
			sword->label = "sword";
			sword->getComponent<ColliderComponent>().id = playerId;
			entitiesManager.addEntity(sword);

			// Gestion des projectiles
			if (std::get<bool>(stats->stats["hasProjectiles"]) && !projectileSent)
			{
				int nbProjectiles = std::get<int>(stats->stats["nbProjectiles"]); // Récupérer le nombre de projectiles
				float angleStep = (nbProjectiles > 1) ? 30.0f / (nbProjectiles - 1) : 0; // Angle d'écart entre les projectiles
				float baseAngle = (nbProjectiles > 1) ? -15.0f : 0; // Centrer les projectiles

				for (int i = 0; i < nbProjectiles; i++)
				{
					float angleOffset = baseAngle + i * angleStep; // Décalage en angle
					Vector2D projDirection = attackDirection.rotate(angleOffset); // Rotation du vecteur directionnel

					Entity* projectile = new Entity(
						StatisticsComponent(0, 0, 0.3, 0, 0),
						TransformComponent(attackPosition.x + p_offsetX, attackPosition.y + p_offsetY, 64, 64, 0.75),
						ProjectileComponent(projDirection),
						ColliderComponent("projectile", 0, 0, 64, 64)
					);

					projectile->label = "projectile";
					projectile->getComponent<ColliderComponent>().id = playerId;
					entitiesManager.addEntity(projectile);
				}

				projectileSent = true;
			}
		}

		bool checkPlayerState()
		{
			if(GameManager::inDeathAnimation)
			{
				for (auto& pair : isBeingPressed) {
					pair.second = false;
				}
				return true;
			}
			
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
			return false;
		}

		void update() override
		{
			bool earlyReturn = checkPlayerState();
			if(earlyReturn)
				return;

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
						if(playerKeys[playerId]["Attack"] == key && (!isSpamming))
						{
							isSpamming = true;
							attack();
						}
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
					case SDLK_SPACE: case SDLK_m:
						isSpamming = false;
						break;
					case SDLK_ESCAPE:
						GameManager::isRunning = false;
						break;
					default:
						break;
				}
			}
		}

		void reset() override
		{
			transform->direction.x = 0;
			transform->direction.y = 0;
			attackDuration = sprite->animations["Attack Down"]["frameTime"] * sprite->animations["Attack Down"]["frames"];
			for (auto& pair : isBeingPressed) {
				pair.second = false;
			}
		}
};