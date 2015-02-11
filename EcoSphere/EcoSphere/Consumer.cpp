#include "Consumer.h"
#include "EcoSystem.h"
Consumer::Consumer(EcoSystem *eco_system) :
Entity(eco_system) {}


bool Consumer::is_producer() const
{
	return false;
}

bool Consumer::is_consumer() const
{
	return true;
}



double Consumer::get_speed() const
{
	return speed;
}

void Consumer::set_speed(double speed)
{
	this->speed = speed;
}
void Consumer::predate()
{
	set_energy(get_energy() - 1);
	Entity *food = eco_system->find_prey(this);
	if (food != NULL)
	{
		if (!eco_system->try_eat(this, food))
		{
			Vector2D target_position = food->get_position();
			Vector2D distance = target_position - this->get_position();
			Vector2D displacement = distance * (1.0 / distance.modulus()) * get_speed();
			//if (displacement.modulus() < get_speed())
			target_position = get_position() + displacement;
			this->set_position(target_position);
		}
	}
	else
		brownian_motion();
}

void Consumer::brownian_motion()
{
	Vector2D unit_vector(1.0, 0.0);
	unit_vector = unit_vector.rotate(EcoSystem::random_angle());
	Vector2D displacement = unit_vector * get_speed();
	Vector2D target_position = get_position() + displacement;
	EcoSystem::prevent_overstep(target_position);
	set_position(target_position);
}

void Consumer::avoid_predator()
{

}
void Consumer::on_eaten()
{}
