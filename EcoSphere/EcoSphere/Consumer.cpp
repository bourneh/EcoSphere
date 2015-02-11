#include "Consumer.h"
#include "EcoSystem.h"
#include "FoodWeb.h"
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
bool Consumer::predate(double speed)
{
	set_speed(speed);
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
		return true;
	}
	return false;
}

bool Consumer::brownian_motion(double speed)
{
	set_speed(speed);
	Vector2D unit_vector(1.0, 0.0);
	unit_vector = unit_vector.rotate(EcoSystem::random_angle());
	Vector2D displacement = unit_vector * get_speed();
	Vector2D target_position = get_position() + displacement;
	EcoSystem::prevent_overstep(target_position);
	set_position(target_position);
	return true;
}

bool Consumer::avoid_predator(double speed)
{
	if (aps.t <= 5)
	{
		++aps.t;
		set_position(get_position() + aps.v);
		return true;
	}
	std::set<Entity*> predators = eco_system->find_all_entity(this, eco_system->get_food_web_instance()->get_predator_set(this));
	if (!predators.empty())
	{
		aps.t = 0;
		set_speed(speed);
		Vector2D d;
		for (std::set<Entity*>::iterator it = predators.begin(); it != predators.end(); it++)
			d += get_position() - (*it)->get_position();
		if (d.modulus() < 1.0)
			d = Vector2D(1, 0).rotate(EcoSystem::random_angle()) * get_speed();
		else
			d = d * (get_speed() / d.modulus());
		set_position(get_position() + d);
		aps.v = d;
		return true;
	}
	return false;
}
void Consumer::on_eaten()
{}
