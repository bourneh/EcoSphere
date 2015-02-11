#include <string>
#include <set>
#include "Vector2D.h"
#include "EcoSystemObject.h"
#include "ecosystem_gdiplus.h"

/*
 *���������DynamicEcoSystemObject��
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

	virtual			std::string get_species_name() const = 0;//���ֵ����ơ�������ţ������������Ｆ����
	virtual			Gdiplus::Image* get_entity_image() const = 0;//�����ͼ��
	virtual Entity* new_entity() const = 0;//���ڲ���һ���µĸ��������
	virtual bool	is_producer() const = 0;
	virtual bool	is_consumer() const = 0;
	virtual int		get_cost_of_being_preyed() const = 0;
	virtual int		get_gain_after_being_preyed() const = 0;


	//������λ�á����䡢��ǰĿ��ȵ�get��set������


	Vector2D get_position() const;
	void set_position(Vector2D position);
	double get_energy() const;
	void set_energy(double energy);
	int		get_age() const;
	void	set_age(int age);
	Entity *get_target() const;
	void set_target(Entity *target);
	void add_predator(Entity *entity);

	
	bool is_alive() const;//�Ƿ���š�
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
