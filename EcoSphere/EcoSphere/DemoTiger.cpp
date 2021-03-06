#include "DemoTiger.h"
#include "EcoSystem.h"
DemoTiger::DemoTiger(EcoSystem *eco_system) :
Consumer(eco_system)
{
	set_alive();
	set_hp(20000);
	set_speed(15.0);
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
	set_age(get_age() + 1);
	set_hp(get_hp() - get_speed() * get_speed() / 2);
	if (!predate(15))
		brownian_motion(10);
}

int	DemoTiger::get_cost_of_being_preyed() const
{
	return 2000;
}
int	DemoTiger::get_gain_after_being_preyed() const
{
	return 3000;
}