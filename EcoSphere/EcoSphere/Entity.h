#include <string>
#include <set>
#include "Vector2D.h"
#include "EcoSystemObject.h"
#include "ecosystem_gdiplus.h"

/*
 *Entity.Inherit from DynamicEcoSystemObject.
*/
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

	//Species name.
	virtual			std::string get_species_name() const = 0;

	//Entity image.
	virtual			Gdiplus::Image* get_entity_image() const = 0;

	//Create a new entity of current type.
	virtual Entity* new_entity() const = 0;

	virtual bool	is_producer() const = 0;
	virtual bool	is_consumer() const = 0;

	virtual int		get_cost_of_being_preyed() const = 0;
	virtual int		get_gain_after_being_preyed() const = 0;




	Vector2D get_position() const;
	void set_position(Vector2D position);
	double get_hp() const;
	void set_hp(double energy);
	int		get_age() const;
	void	set_age(int age);
	Entity *get_target() const;
	void set_target(Entity *target);
	void add_predator(Entity *entity);

	
	bool is_alive() const;
	void set_alive();
	void set_dead();
protected:
	bool valid;

	//Entity that this entity is staring at.
	Entity *target;

	//Entities that are staring at this entity.
	std::set<Entity*> predators;
private:
	int age;
	double hp;
	Vector2D position;

};

#endif // DS_ECOSYSTEM_ENTITY
