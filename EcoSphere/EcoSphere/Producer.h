#include "Entity.h"
#ifndef DS_ECOSYSTEM_PRODUCER
#define DS_ECOSYSTEM_PRODUCER

class Producer : public Entity
{
public:
	virtual void on_tick();
	virtual bool is_producer() const;
	virtual bool is_consumer() const;
	unsigned int get_growth_cycle() const;
	void set_growth_cycle(unsigned int growth_cycle);
protected:
	unsigned int growth_time;
	
	Producer(EcoSystem *eco_system);
	virtual void multiply();
private:
	unsigned int growth_cycle;
};

#endif // DS_ECOSYSTEM_PRODUCER
