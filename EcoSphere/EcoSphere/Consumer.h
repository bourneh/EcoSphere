#include "Vector2D.h"
#include "Entity.h"
/*
 *Consumer.
 *Mostly animals.
*/
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
	virtual bool predate(double speed);
	virtual bool brownian_motion(double speed);
	virtual bool avoid_predator(double speed);
	virtual void multiply();
protected:
	Consumer(EcoSystem *eco_system);

	//A structure used in avoid predator behavior
	struct avoid_predator_structure
	{
		int t;
		Vector2D v;
		bool b;
		avoid_predator_structure() : t(6) {}
	}aps;
private:
	double speed;
};

#endif // DS_ECOSYSTEM_COMSUMER
