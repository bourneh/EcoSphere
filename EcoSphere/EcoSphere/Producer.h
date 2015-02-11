#include "Entity.h"
/*
 *�����ߡ�
 *Ϊʵ�ֶ������������Ľӿڡ�
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
