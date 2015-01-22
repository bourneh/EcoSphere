#include <string>
#include "Vector2D.h"
#include "EcoSystemObject.h"
#include "ecosystem_gdiplus.h"
#ifndef DS_ECOSYSTEM_ENTITY
#define DS_ECOSYSTEM_ENTITY

class EcoSystem;
class FoodWeb;
class Environment;

class Entity : public DynamicEcoSystemObject
{
public:
	Entity(EcoSystem *eco_system);
	virtual ~Entity();

	virtual			std::string get_species_name() const = 0;
	virtual			Gdiplus::Image* get_entity_image() const = 0;
	virtual Entity* new_entity() const = 0;
	virtual bool	is_producer() const = 0;
	virtual bool	is_consumer() const = 0;

	bool is_valid() const; 
	void set_valid(bool valid);
	Vector2D get_position() const;
	void set_position(Vector2D position);
	double get_energy() const;
	void set_energy(double energy);
protected:
	bool valid;
private:
	double energy;
	Vector2D position;

};

#endif // DS_ECOSYSTEM_ENTITY
