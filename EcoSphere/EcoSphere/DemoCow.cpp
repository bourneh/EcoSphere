#include "DemoCow.h"
#include "EcoSystem.h"
#include <iostream>
DemoCow::DemoCow(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_alive();
	set_energy(15000);
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
	set_energy(get_energy() - get_speed() * get_speed() / 2);
	predate();
}

int	DemoCow::get_cost_of_being_preyed() const
{
	return 500;
}
int	DemoCow::get_gain_after_being_preyed() const
{
	return 1000;
}