#include "Vector2D.h"
#include "Entity.h"

#ifndef DS_ECOSYSTEM_COMSUMER
#define DS_ECOSYSTEM_COMSUMER

class Consumer : public Entity
{
public:
	virtual bool is_producer() const;
	virtual bool is_consumer() const;

	int get_strength() const;
	void set_strength(int strength);
	Vector2D get_velocity() const;
	void set_velocity(double velocity);
	
	//virtual void avoid_predator();
protected:
	Consumer(EcoSystem *eco_system);
private:
	int strength;
	Vector2D velocity;
};

#endif // DS_ECOSYSTEM_COMSUMER
