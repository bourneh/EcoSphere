#include "Environment.h"
#include "EcoSystem.h"
#include "Producer.h"
Environment::Environment(EcoSystem *eco_system) :
DynamicEcoSystemObject(eco_system) 
{
	set_brightness(15.0);
	set_temperature(25.0);
	set_relative_humidity(0.30);
	set_precipitation(0.0);
	set_default_producer(NULL);
}

Environment::~Environment()
{}

void Environment::render_environment_background(Gdiplus::Graphics *g) const
{
	g->Clear(Gdiplus::Color(0, 255, 0));
}

void Environment::render_environment_effects(Gdiplus::Graphics *g) const
{
}

void Environment::on_tick()
{
	spawn_producer();
}

void Environment::spawn_producer()
{
	if (default_producer != NULL)
		if (EcoSystem::random_double() > 0.8)
			for (int i = 0; i < 20; i++)
				eco_system->spawn_entity(default_producer->new_entity());
}

double Environment::get_brightness() const
{
	return brightness;
}

void Environment::set_brightness(double brightness)
{
	this->brightness = brightness;
}


double Environment::get_temperature() const
{
	return temperature;
}

void Environment::set_temperature(double temperature)
{
	this->temperature = temperature;
}


double Environment::get_relative_humidity() const
{
	return relative_humidity;
}

void Environment::set_relative_humidity(double relative_humidity)
{
	this->relative_humidity = relative_humidity;
}


double Environment::get_precipitation() const
{
	return precipitation;
}

void Environment::set_precipitation(double precipitation)
{
	this->precipitation = precipitation;
}

void Environment::set_default_producer(Producer *producer)
{
	this->default_producer = producer;
}