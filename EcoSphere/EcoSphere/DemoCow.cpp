#include "DemoCow.h"
#include "EcoSystem.h"
#include <iostream>
DemoCow::DemoCow(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_alive();
	set_hp(15000);
	set_speed(0.0);
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
	set_age(get_age() + 1);
	set_hp(get_hp() - get_speed() * get_speed() / 2);
	if(!avoid_predator(10))
		if(!predate(5))
			brownian_motion(3);
}

int	DemoCow::get_cost_of_being_preyed() const
{
	return 500;
}
int	DemoCow::get_gain_after_being_preyed() const
{
	return 1000;
}