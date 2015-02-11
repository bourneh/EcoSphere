#include "Vector2D.h"
#include "Entity.h"

#ifndef DS_ECOSYSTEM_COMSUMER
#define DS_ECOSYSTEM_COMSUMER

class Consumer : public Entity
{
public:
	virtual bool is_producer() const;
	virtual bool is_consumer() const;

	double	get_speed() const;
	void	set_speed(double speed);
	
	//reactions
	virtual void on_eaten();

	//behaviors
	virtual void predate();
	virtual void brownian_motion();
	virtual void avoid_predator();
protected:
	Consumer(EcoSystem *eco_system);
private:
	double speed;
};

#endif // DS_ECOSYSTEM_COMSUMER
