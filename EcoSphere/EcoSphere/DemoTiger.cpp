#include "DemoTiger.h"
#include "EcoSystem.h"
DemoTiger::DemoTiger(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_valid(true);
	set_energy(10000);
}

DemoTiger::~DemoTiger() {}

std::string DemoTiger::get_species_name() const
{
	return "hehetiger";
}

Gdiplus::Image *DemoTiger::get_entity_image() const
{
	static Gdiplus::Image *img = Gdiplus::Image::FromFile(L".\\images\\demotiger.png");
	return img;
}

Entity *DemoTiger::new_entity() const
{
	return new DemoTiger(eco_system);
}

void DemoTiger::on_tick()
{
	const static double SPEED = 15.0;
	set_energy(get_energy() - 1);
	Entity *food = eco_system->find_entity(this, "hehecow");
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
