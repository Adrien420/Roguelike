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

// Fonction incrémentant l'id lorsqu'un nouveau composant est ajouté à une entité
// Le mot-clé inline permet ici à la fonction d'être redéfinie dans plusieurs headers sans causer d'erreur
inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

// Fonction retournant l'identifiant du type de composant
template <typename T> inline ComponentID getComponentTypeID()
{
	// Vérifie que le type T est bien Uun Component ouo une dérivée
    static_assert (std::is_base_of<Component, T>::value, "");
    // Identifiant statique, qui ne sera donc pas réinitialisé si la fonction est appelée avec un composant déjà ajouté
    // Permet donc d'avoir un id pour chaque type de composant
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

// Variable permettant de savoir si un composant est présent ou non dans l'entité
using ComponentBitSet = std::bitset<maxComponents>;
//Tableau de composants présents dans l'entité, pour pouvoir les récupèrer depuis d'autres programmes
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
        // std::unique_ptr permet de gérer automatiquement la mémoire (pointeur supprimé si hors de portée notamment)
        // components est un vecteur d'adresses des composants présents dans l'entité
        std::vector<std::unique_ptr<Component>> components;

        ComponentArray componentArray;
        ComponentBitSet componentBitset;

    public:
        std::string label = "";
        // Identifiant utilisé pour savoir de quel joueur il s'agit
        std::string playerId = "";

        // Constructeur variadique de l'entité : prend un nombre variable de constructeurs de composants
        // Les paramètres des constructeurs de composant sont passés à la méthode addComponent graĉe à std::forward
        // Targs&&... permet de récupérer aussi bien des références que des valeurs
        // Le ", ..." est une fold expression, qui permet d'appeler addComponent pour chaque constructeur passé en paramètre
        template <typename... TArgs>
        Entity(TArgs&&... args) {
            (addComponent<TArgs>(std::forward<TArgs>(args)), ...);
        }

        ~Entity() {
            components.clear();
        }

        // Méthode d'ajout de composant
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

        // Méthode permettant de vérifier si un composant est déjà ajouté à l'entité
        template <typename T> bool hasComponent() const
        {
            return componentBitset[getComponentTypeID<T>()];
        }

        // Méthode permettant de récupérer un composant depuis un autre composant / programme
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

        // Indique si l'entité est active, si elle est inactive elle sera supprimée de l'entityManager par la méthode refresh
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

        // Méthode supprimant les entités devenues inactives
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