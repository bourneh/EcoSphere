#include "Entity.h"
#include "Vector2D.h"
#include <iostream>
Entity::Entity(EcoSystem *eco_system) :
DynamicEcoSystemObject(eco_system)
{
	set_alive();
	set_age(1);
	target = NULL;
}
Entity::~Entity() 
{
}

bool Entity::is_alive() const
{
	return valid;
}

void Entity::set_alive()
{
	this->valid = true;
}

void Entity::set_dead()
{
	this->valid = false;
	std::set<Entity*>::iterator it;
	for (it = predators.begin(); it != predators.end(); it++)
		(*it)->target = NULL;
	if (target != NULL)
		target->predators.erase(this);
}

double Entity::get_hp() const
{
	return hp;
}

void Entity::set_hp(double hp)
{
	this->hp = hp;
}

Vector2D Entity::get_position() const
{
	return position;
}

void Entity::set_position(Vector2D position)
{
	this->position = position;
}

int Entity::get_age() const
{
	return age;
}

void Entity::set_age(int age)
{
	this->age = age;
}

Entity *Entity::get_target() const
{
	return target;
}
void Entity::set_target(Entity *target)
{
	this->target = target;
}
void Entity::add_predator(Entity *entity)
{
	predators.insert(entity);
}
