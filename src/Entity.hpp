#include <vector>
#include <memory>
#include "Component.hpp"

class Entity
{
    private:
        std::vector<std::unique_ptr<Component>> components;

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

            // Initialisation du composant
            c->init();
            return *c;
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

        template <typename T> bool hasComponent() const
        {
            return componentBitset[getComponentTypeID<T>()];
        }

        template<typename T> T& getComponent() const
        {
            auto ptr(componentArray[getComponentTypeID<T>()]);
            return *static_cast<T*>(ptr);
        }
};