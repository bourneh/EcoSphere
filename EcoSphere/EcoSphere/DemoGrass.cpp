#include "DemoGrass.h"
#include <iostream>
DemoGrass::DemoGrass(EcoSystem *eco_system) :
Producer(eco_system)
{
	set_energy(500);
	set_growth_cycle(5000);
	this->set_valid(true);
}
Gdiplus::Image *DemoGrass::get_entity_image() const
{
	static Gdiplus::Image *img = Gdiplus::Image::FromFile(L".\\images\\demograss.png");
	return img;
}

Entity *DemoGrass::new_entity() const
{
	return new DemoGrass(get_eco_system_instance());
}

std::string DemoGrass::get_species_name() const
{
	return "hehegrass";
}

void DemoGrass::on_tick()
{
	Producer::on_tick();
}