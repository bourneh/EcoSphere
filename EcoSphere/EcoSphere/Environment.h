#include "ecosystem_gdiplus.h"
#include "EcoSystemObject.h"
#include "Vector2D.h"
#ifndef DS_ECOSYSTEM_ENVIRONMENT
#define DS_ECOSYSTEM_ENVIRONMENT

class EcoSystem;
class Entity;
class Producer;
class FoodWeb;
class VectorField;

class Environment : public DynamicEcoSystemObject
{
public:
	Environment(EcoSystem *eco_system);
	virtual ~Environment();
	virtual void    render_environment_background(Gdiplus::Graphics *g) const;
	virtual void    render_environment_effects(Gdiplus::Graphics *g) const;
	virtual void	on_tick();
	virtual void	spawn_producer();
	double      get_brightness() const;
	void        set_brightness(double brightness);
	double      get_temperature() const;
	void        set_temperature(double temperature);
	double      get_relative_humidity() const;
	void        set_relative_humidity(double relative_humidity);
	double      get_precipitation() const;
	void        set_precipitation(double precipitation);
	Producer *get_default_producer() const;
	void		set_default_producer(Producer *producer);

protected:
private:
	double brightness, temperature, relative_humidity, precipitation;// brightness(0 - 100), tempreature(degree celsius), relative Humidity(0.0 ~ 1.0), precipitation(millimeter)
	Producer *default_producer;
};

#endif // DS_ECOSYSTEM_ENVIRONMENT
