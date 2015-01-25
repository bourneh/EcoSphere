#include "DemoCow.h"
#include "EcoSystem.h"
#include <iostream>
DemoCow::DemoCow(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_valid(true);
	set_energy(5000);
	set_speed(5.0);
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
	predate();

	if (get_energy() < 300)
		set_valid(false);
}
