#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <SFML/Graphics.hpp>

/*
* CES a better design  option
*
* Components:
*	[ NPC ]
*   [ Enemy ]
*   [Friendly]
*   [ Armord ]
*   [ WithGun ]
*	...                   |
*
*  Entities:
*	[[ Skeleton ]] = [ Enemy ] + [ NPC ]
*   [[ Paladin ]] = [Friendly] + [Armord]
*   [[ Player ]] = [Friendly] + [Armored] + [WithGun]
*   ...
*/


/*
	[ Manager ]								/->update()
		 |				   /-[ Component]---|
		 \- [ Entity ] -- |                 \->draw()
		 |                |
		 |				  |
		 | 				  |                  /->update()
		 |				   \-[Component]----|
		 |				                     \->draw()
		 |
		 |                   				/->update()
		 |				   /-[ Component]---|
		 \- [ Entity ] -- |                 \->draw()
						  |
						  |
						  |                  /->update()
						   \-[Component]----|
											 \->draw()

*/


namespace CompositionArkanoid
{
	class Entity;
	struct Component;
	class Manager;

	using ComponentID = std::size_t;

	using Group = std::size_t;
	//
	inline ComponentID getUniqueComponentID() noexcept
	{
		static ComponentID lastID{ 0u };
		return lastID++;
	}
	//
	template<typename T>inline ComponentID getComponentTypeID() noexcept
	{
		static ComponentID typeID{ getUniqueComponentID() };
		return typeID;
	}

	constexpr std::size_t maxComponents = 32;

	using ComponentBitSet = std::bitset<maxComponents>;
	using ComponentArray = std::array<Component*, maxComponents>;

	constexpr std::size_t maxGroups = 32;
	using GroupBitSet = std::bitset<maxGroups>;

	struct Component {
		Entity* entity{ nullptr };

		virtual void init() {};
		virtual void update(float dt) {};
		virtual void input(sf::Event event) {};
		virtual void draw(sf::RenderWindow& window) {};
		virtual ~Component() {};
	};
	//
	class Entity
	{

	private:
		Manager& manager;

		bool alive{ true };
		std::vector<std::unique_ptr<Component>> components;

		ComponentArray componentArray;
		ComponentBitSet componentBitset;

		GroupBitSet groupBitset;

	public:
		Entity(Manager& mng) :manager(mng) {}

		bool isAlive() const { return alive; }
		void destroy() { alive = false; }

		void update(float dt) {
			for (auto& c : components) {
				c->update(dt);
			}
		}

		void draw(sf::RenderWindow& window) {
			for (auto& c : components) {
				c->draw(window);
			}
		}

		void input(sf::Event event) {
			for (auto& c : components) {
				c->input(event);
			}
		}

		bool hasGroup(Group mGroup)const noexcept
		{
			return groupBitset[mGroup];
		}

		void addGroup(Group mGroup)noexcept;
		void deleteGroup(Group mGroup)noexcept
		{
			groupBitset[mGroup] = false;
		}


		template<typename T> bool hasComponent()const
		{
			return componentBitset[getComponentTypeID<T>];
		}

		template<typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{

			T* c(new T(std::forward<TArgs>(mArgs)...));

			c->entity = this;

			std::unique_ptr<Component> uPtr{ c };

			components.emplace_back(std::move(uPtr));

			componentArray[getComponentTypeID<T>()] = c;
			componentBitset[getComponentTypeID<T>()] = true;

			c->init();

			return *c;
		}


		template<typename T> T& getComponent() const
		{
			auto ptr(componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

	};

	class Manager {

	private:
		std::vector<std::unique_ptr<Entity>> entities;

		std::array<std::vector<Entity*>, maxGroups> groupedEntities;

	public:
		void update(float dt)
		{
			entities.erase(
				std::remove_if(std::begin(entities), std::end(entities),
					[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isAlive();
			}), std::end(entities));


			for (auto& e : entities)
			{
				e->update(dt);
			}

		}

		void draw(sf::RenderWindow& window)
		{
			for (auto& e : entities)
			{
				e->draw(window);
			}

		}

		void input(sf::Event event) {
			for (auto& e : entities) {
				e->input(event);
			}
		}

		void refresh()
		{

			for (auto i(0u); i < maxGroups; ++i)
			{
				auto& v(groupedEntities[i]);

				v.erase(std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
				{
					return !mEntity->isAlive() || !mEntity->hasGroup(i);
				}), std::end(v));

			}
			//		   			 
			entities.erase(std::remove_if(std::begin(entities),
				std::end(entities),
				[](const std::unique_ptr<Entity> &mEntity)
			{
				return !mEntity->isAlive();
			}), std::end(entities));

		}

		Entity& addEntity()
		{
			Entity* e{ new Entity{*this} };
			std::unique_ptr<Entity> uPtr{ e };
			entities.emplace_back(std::move(uPtr));
			return *e;
		}

		void addToGroup(Entity* mEntity, Group mGroup)
		{
			groupedEntities[mGroup].emplace_back(mEntity);
		}

		std::vector<Entity*>& getEntitiesByGroup(Group mGroup)
		{
			return groupedEntities[mGroup];
		}


	};

	void Entity::addGroup(Group mGroup)noexcept
	{
		groupBitset[mGroup] = true;
		manager.addToGroup(this, mGroup);
	}


};