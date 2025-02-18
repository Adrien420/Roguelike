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
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public: 
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}
};

class Entity
{
    private:
        bool active = true;
        std::vector<std::unique_ptr<Component>> components;

        ComponentArray componentArray;
        ComponentBitSet componentBitset;
        GroupBitset groupBitset;

    public:
        // 🌟 Constructeur simplifié : ajoute plusieurs composants directement
        template <typename... TArgs>
        Entity(TArgs&&... args) {
            (addComponent(std::forward<TArgs>(args)), ...);
        }

        // 🌟 Version originale de `addComponent`
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

        bool isActive() const { return active; }

        void destroy() { active = false; }

        bool hasGroup(Group mGroup)
        {
            return groupBitset[mGroup];
        }

        void addGroup(Group mGroup);

        void delGroup(Group mGroup)
        {
            groupBitset[mGroup] = false;
        }
};