#include "Consumer.h"

Consumer::Consumer(EcoSystem *eco_system) :
Entity(eco_system) {}


bool Consumer::is_producer() const
{
	return true;
}

bool Consumer::is_consumer() const
{
	return false;
}


int Consumer::get_strength() const
{
	return strength;
}

void Consumer::set_strength(int strength)
{
	this->strength = strength;
}

Vector2D Consumer::get_velocity() const
{
	return velocity;
}

void Consumer::set_velocity(Vector2D velocity)
{
	this->velocity = velocity;
}
