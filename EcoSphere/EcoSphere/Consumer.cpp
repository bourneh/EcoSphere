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

/*
 *Predation.Parameter speed is the speed of this animal while it's chasing its prey.
 *Returns true if this animal found a prey.
 */
bool Consumer::predate(double speed)
{
	set_speed(speed);
	Entity *food = eco_system->find_prey(this);
	if (food != NULL)
	{
		//Wooooooooooow!!!I see my delicious dinner near me!!!
		if (!eco_system->try_eat(this, food))
		{
			/*
			 *Suppose the position of the prey is target_position.
			 *The direction that this animal should run toward is the direction of [(target_position) -(position of this animal)]
			 *Normalize the vector [(target_position) -(position of this animal)] then times the speed we get displacement
			 *{(position of this animal) + displacement} is the next position of this animal 
			 */
			Vector2D target_position = food->get_position();
			Vector2D distance = target_position - this->get_position();
			Vector2D displacement = distance * (1.0 / distance.modulus()) * get_speed();
			target_position = get_position() + displacement;
			this->set_position(target_position);
		}
		return true;
	}
	return false;//404 Prey not found.
}

/*
 *Walk toward a random direction with a specified speed
 */
bool Consumer::brownian_motion(double speed)
{
	set_speed(speed);
	/*
	 *First generate a unit vector.
	 *Rotate it by a random angle.
	 *Times the speed of this animal.
	 *Then we get displacement.
	 *[(position of this animal) + displacement] is the next position of this animal .
	 */
	Vector2D unit_vector(1.0, 0.0);
	unit_vector = unit_vector.rotate(EcoSystem::random_angle());
	Vector2D displacement = unit_vector * get_speed();
	Vector2D target_position = get_position() + displacement;
	EcoSystem::prevent_overstep(target_position);
	set_position(target_position);
	return true;
}

/*
 *Behavior of avoiding the predator of this animal.
 *It's vital for an animal to avoid its predators.
 *Otherwise it will die.
 *Returns true if this animal realize there are predators around it.
 */
bool Consumer::avoid_predator(double speed)
{
	/*
	 *If it found predators around it, it would run toward one direction for 5 ticks.
	 */
	if (aps.t <= 5)
	{
		++aps.t;
		set_position(get_position() + aps.v);
		return true;
	}
	int predators_count = eco_system->get_around_predators_count(this);
	if (predators_count != 0)
	{
		/*
		 *If there are predators around it!!
		 *Suppose there are n predators around and the position vector of the i-th predator is pos_i
		 *The direction of this animal to run away is the direction of
		 *
		 *       n
		 * v = sigma (position of this animal - pos_i)
		 *      i=1
		 *
		 *Simplify it, then 
		 *
		 *                                      n
		 *v = n * (position of this animal) - sigma (pos_i)
		 *                                     i=1
		 *
		 *and the term [sigma (pos_i)] has been calculated by EcoSystem
		 */
		aps.t = 0;
		set_speed(speed);
		Vector2D predator_pos_sum = eco_system->get_around_predators_position_sum(this);
		Vector2D d = predators_count * get_position() - predator_pos_sum;


		if (d.modulus() < 1.0)//Avoid divided-by zero exception
			d = Vector2D(1, 0).rotate(EcoSystem::random_angle()) * get_speed();
		else
			d = d * (get_speed() / d.modulus());
		set_position(get_position() + d);
		aps.v = d;
		return true;
	}
	return false;
}

/*
 *Multiply.Generate a baby of this animal.
 */
void Consumer::multiply()
{
	eco_system->spawn_entity(new_entity());
}

/*
 *Some animals would take some actions when they are about to be eaten.
 *For example, scream to warn other animals around it.
 */
void Consumer::on_eaten()
{}
