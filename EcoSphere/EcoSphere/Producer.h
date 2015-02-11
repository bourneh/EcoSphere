#include "Entity.h"
/*
 *生产者。
 *为实现多种生产者留的接口。
*/
#ifndef DS_ECOSYSTEM_PRODUCER
#define DS_ECOSYSTEM_PRODUCER

class Producer : public Entity
{
public:
	virtual void on_tick();
	virtual bool is_producer() const;
	virtual bool is_consumer() const;
protected:
	Producer(EcoSystem *eco_system);
	virtual void multiply();
};

#endif // DS_ECOSYSTEM_PRODUCER
