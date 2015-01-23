#include "DemoCow.h"
#include "EcoSystem.h"
#include <iostream>
DemoCow::DemoCow(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_valid(true);
	set_energy(5000);
}

DemoCow::~DemoCow() {}

std::string DemoCow::get_species_name() const
{
	return "hehecow";
}

Gdiplus::Image *DemoCow::get_entity_image() const
{
	static Gdiplus::Image *img = Gdiplus::Image::FromFile(L".\\images\\democow.png");
	return img;
}

Entity *DemoCow::new_entity() const
{
	return new DemoCow(eco_system);
}

void DemoCow::on_tick()
{
	const static double SPEED = 5.0;
	set_energy(get_energy() - 1);
	Entity *food = eco_system->find_entity(this, "Grass");
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
			Vector2D displacement = distance * (1.0 / distance.modulus()) * SPEED;
			this->set_position(get_position() + displacement);
		}
		//eco_system->try_eat(this, food);
	}
	else
	{
		Vector2D unit_vector(1.0, 0.0);
		unit_vector = unit_vector.rotate(EcoSystem::random_angle());
		Vector2D displacement = unit_vector * SPEED;
		Vector2D target_position = get_position() + displacement;
		if (target_position.x > 0.0 && target_position.x < EcoSystem::DEFAULT_WIDTH
			&& target_position.y > 0.0 && target_position.y < EcoSystem::DEFAULT_HEIGHT)
			this->set_position(target_position);
	}

	if (get_energy() < 300)
		set_valid(false);
}
