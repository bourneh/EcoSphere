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

double Consumer::get_speed() const
{
	return speed;
}

void Consumer::set_speed(double speed)
{
	this->speed = speed;
}
