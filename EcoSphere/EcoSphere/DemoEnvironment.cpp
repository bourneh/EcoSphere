#include "DemoEnvironment.h"
DemoEnvironment::DemoEnvironment(EcoSystem *eco_system) :
Environment(eco_system)
{
	set_brightness(15.0);
	set_temperature(25.0);
	set_relative_humidity(0.30);
	set_precipitation(0.0);
}

void DemoEnvironment::on_tick()
{}

void DemoEnvironment::render_environment_background(Gdiplus::Graphics *g) const
{
	g->Clear(Gdiplus::Color(0, 255, 0));
}