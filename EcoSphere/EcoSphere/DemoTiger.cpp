#include "DemoTiger.h"
#include "EcoSystem.h"
DemoTiger::DemoTiger(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_alive();
	set_energy(100000);
	set_speed(15.0);
	set_strength(15);
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
	predate();

	if (get_energy() < 300)
		set_died();
}
