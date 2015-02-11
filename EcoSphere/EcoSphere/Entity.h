#include <string>
#include <set>
#include "Vector2D.h"
#include "EcoSystemObject.h"
#include "ecosystem_gdiplus.h"

/*
 *生物。派生自DynamicEcoSystemObject。
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

	virtual			std::string get_species_name() const = 0;//物种的名称。比如鼠牛虎兔龙蛇马羊猴鸡狗猪。
	virtual			Gdiplus::Image* get_entity_image() const = 0;//动物的图像。
	virtual Entity* new_entity() const = 0;//用于产生一个新的该物种生物。
	virtual bool	is_producer() const = 0;
	virtual bool	is_consumer() const = 0;
	virtual int		get_cost_of_being_preyed() const = 0;
	virtual int		get_gain_after_being_preyed() const = 0;


	//下面是位置、年龄、当前目标等的get和set函数。


	Vector2D get_position() const;
	void set_position(Vector2D position);
	double get_energy() const;
	void set_energy(double energy);
	int		get_age() const;
	void	set_age(int age);
	Entity *get_target() const;
	void set_target(Entity *target);
	void add_predator(Entity *entity);

	
	bool is_alive() const;//是否活着。
	void set_alive();
	void set_dead();
protected:
	bool valid;
	Entity *target;
	std::set<Entity*> predators;
private:
	int age;
	double energy;
	Vector2D position;

};

#endif // DS_ECOSYSTEM_ENTITY
