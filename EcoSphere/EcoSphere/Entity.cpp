#include "Entity.h"
#include "Vector2D.h"
Entity::Entity(EcoSystem *eco_system) :
DynamicEcoSystemObject(eco_system)
{
	valid = false;
}
Entity::~Entity() {}

bool Entity::is_valid() const
{
	return valid;
}

void Entity::set_valid(bool valid)
{
	this->valid = valid;
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
