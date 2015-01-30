#include "Entity.h"
#include "Vector2D.h"
Entity::Entity(EcoSystem *eco_system) :
DynamicEcoSystemObject(eco_system)
{
	set_alive();
}
Entity::~Entity() {}

bool Entity::is_alive() const
{
	return valid;
}

void Entity::set_alive()
{
	this->valid = true;
}

void Entity::set_died()
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
