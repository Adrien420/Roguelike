#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public: 
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
    virtual void reset() {}
	virtual ~Component() {}
};

class Entity
{
    private:
        bool active = true;
        std::vector<std::unique_ptr<Component>> components;

        ComponentArray componentArray;
        ComponentBitSet componentBitset;

    public:
        std::string label = "";
        template <typename... TArgs>
        Entity(TArgs&&... args) {
            (addComponent<TArgs>(std::forward<TArgs>(args)), ...);
        }

        ~Entity() {
            components.clear();
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs) {
            // Création dynamique du composant avec les arguments passés
            T* c = new T(std::forward<TArgs>(mArgs)...);
            c->entity = this;
            std::unique_ptr<Component> uPtr { c };
            components.emplace_back(std::move(uPtr));

            componentArray[getComponentTypeID<T>()] = c;
            componentBitset[getComponentTypeID<T>()] = true;

            // Initialisation du composant
            c->init();
            return *c;
        }

        template <typename T> bool hasComponent() const
        {
            return componentBitset[getComponentTypeID<T>()];
        }

        template<typename T> T& getComponent() const
        {
            auto ptr(componentArray[getComponentTypeID<T>()]);
            return *static_cast<T*>(ptr);
        }

        void update()
        {
            for (auto& c : components) c->update();
        }
        void draw() 
        {
            for (auto& c : components) c->draw();
        }

         void reset() 
        {
            for (auto& c : components) c->reset();
        }

        bool isActive() const { return active; }

        void destroy() { active = false; }
};

class EntitiesManager
{
    public:
        std::vector<Entity*> entities;

        ~EntitiesManager() {
            for (Entity* e : entities) {
                if(e)
                    delete e;  // Libère la mémoire allouée dynamiquement
            }
            entities.clear();  // Vide le vecteur après avoir libéré la mémoire
        }

        void update()
        {
            int i = 0;
            for (size_t j = 0; j < entities.size(); ++j) {
                if (entities[j]) {  // Vérifie si l'entité est valide
                    entities[j]->update();
                }
            }
            refresh(); // Supprime les entités inactives après l'update
        }

        void draw()
        {
            for (auto& e : entities) e->draw();
        } 

        void reset()
        {
            for (auto& e : entities) e->reset();
        }

        void refresh()
        {
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [](Entity* e) { 
                    if (!e->isActive()) {
                        delete e; // Libère la mémoire
                        e = nullptr;
                        return true; // Supprime l'élément du vecteur
                    }
                    return false;
                }), 
                entities.end());
        }

        void addEntity(Entity* entity)
        {
            entities.emplace_back(entity);
        }
};

#endif