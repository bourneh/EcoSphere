#include "Entity.h"
#include "Vector2D.h"
#include <iostream>
Entity::Entity(EcoSystem *eco_system) :
DynamicEcoSystemObject(eco_system)
{
	set_alive();
	target = NULL;
}
Entity::~Entity() 
{
	std::set<Entity*>::iterator it;
	for (it = ts.begin(); it != ts.end(); it++)
		(*it)->target = NULL;
	if (target != NULL)
		target->ts.erase(this);
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
}

double Entity::get_energy() const
{
	return energy;
}

void Entity::set_energy(double energy)
{
	this->energy = energy;
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
void Entity::add_ts(Entity *entity)
{
	ts.insert(entity);
}
