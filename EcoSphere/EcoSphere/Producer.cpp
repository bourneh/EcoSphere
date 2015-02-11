#include "Producer.h"
#include "EcoSystem.h"

Producer::Producer(EcoSystem *eco_system) :
Entity(eco_system) {}

bool Producer::is_producer() const
{
	return true;
}

bool Producer::is_consumer() const
{
	return false;
}

void Producer::on_tick()
{}

void Producer::multiply()
{
	Producer *new_producer = (Producer*)this->new_entity();
	Vector2D spawn_position = EcoSystem::random_double() * Vector2D(5, 0).rotate(EcoSystem::random_angle()) + get_position();
	eco_system->spawn_entity(new_producer, spawn_position);
}