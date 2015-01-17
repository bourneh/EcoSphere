#include "Environment.h"

void Environment::render_environment_background(Gdiplus::Graphics *g) const
{
}

void Environment::render_environment_effects(Gdiplus::Graphics *g) const
{
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
