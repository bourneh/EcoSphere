#include "Consumer.h"
#include "EcoSystem.h"
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

void Consumer::predate()
{
	set_energy(get_energy() - 1);
	Entity *food = eco_system->find_prey(this);
	if (food != NULL)
	{
		Vector2D target_position = food->get_position();
		Vector2D distance = target_position - this->get_position();
		if (distance.modulus() < 20.0)
		{
			food->set_valid(false);
			set_energy(get_energy() + 0.20 * food->get_energy());
		}
		else
		{
			Vector2D displacement = distance * (1.0 / distance.modulus()) * get_speed();
			this->set_position(get_position() + displacement);
		}
		eco_system->try_eat(this, food);
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
	if (target_position.x > 0.0 && target_position.x < EcoSystem::DEFAULT_WIDTH
		&& target_position.y > 0.0 && target_position.y < EcoSystem::DEFAULT_HEIGHT)
		this->set_position(target_position);
}