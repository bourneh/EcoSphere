#include "Grass.h"
#include "Environment.h"
#include <iostream>
Grass::Grass(EcoSystem *eco_system) :
Producer(eco_system)
{
	set_age(0);
	set_energy(100);
	set_growth_cycle(5000);
}
Gdiplus::Image *Grass::get_entity_image() const
{
	static Gdiplus::Image *img = Gdiplus::Image::FromFile(L".\\images\\grass.png");
	return img;
}

Entity *Grass::new_entity() const
{
	return new Grass(get_eco_system_instance());
}

std::string Grass::get_species_name() const
{
	return "Grass";
}

void Grass::on_tick()
{
	set_age(get_age() + 1);
	if (get_age() > LIFE_SPAN)
		set_died();
}