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
	virtual bool predate(double speed);
	virtual bool brownian_motion(double speed);
	virtual bool avoid_predator(double speed);
protected:
	Consumer(EcoSystem *eco_system);
private:
	double speed;

	struct avoid_predator_str
	{
		int t;
		Vector2D v;
		bool b;
		avoid_predator_str() : t(0) {}
	}aps;
};

#endif // DS_ECOSYSTEM_COMSUMER
