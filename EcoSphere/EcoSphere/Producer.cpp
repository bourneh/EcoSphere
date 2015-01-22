#include "Producer.h"
#include "EcoSystem.h"

Producer::Producer(EcoSystem *eco_system) :
Entity(eco_system), growth_time(0) {}

bool Producer::is_producer() const
{
	return true;
}

bool Producer::is_consumer() const
{
	return false;
}

void Producer::on_tick()
{
	++growth_time;
	if (growth_time == growth_cycle)
	{
		growth_time = 0;
		multiply();
	}
}
unsigned int Producer::get_growth_cycle() const
{
	return growth_cycle;
}
void Producer::set_growth_cycle(unsigned int growth_cycle)
{
	this->growth_cycle = growth_cycle;
}

void Producer::multiply()
{
	Producer *new_producer = (Producer*)this->new_entity();
	eco_system->spawn_entity(new_producer);
}