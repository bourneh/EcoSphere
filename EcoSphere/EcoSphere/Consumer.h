#include "Vector2D.h"
#include "Entity.h"

#ifndef DS_ECOSYSTEM_COMSUMER
#define DS_ECOSYSTEM_COMSUMER

class Consumer : public Entity
{
public:
	virtual bool is_producer() const;
	virtual bool is_consumer() const;

	int		get_strength() const;
	void	set_strength(int strength);
	double	get_speed() const;
	void	set_speed(double speed);
	
	virtual void predate();
	virtual void brownian_motion();
	//virtual void avoid_predator();
protected:
	Consumer(EcoSystem *eco_system);
private:
	int strength;
	double speed;
};

#endif // DS_ECOSYSTEM_COMSUMER
