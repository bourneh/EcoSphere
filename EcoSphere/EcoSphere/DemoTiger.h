#include "Consumer.h"

#ifndef DS_ECOSYSTEM_BASIC_CARNIVORE_CONSUMER
#define DS_ECOSYSTEM_BASIC_CARNIVORE_CONSUMER

class DemoTiger : public Consumer
{
public:
	DemoTiger(EcoSystem *eco_system);
	virtual ~DemoTiger();

	virtual std::string get_species_name() const;
	virtual Gdiplus::Image* get_entity_image() const;
	virtual Entity* new_entity() const;
	virtual int		get_cost_of_being_preyed() const;
	virtual int		get_gain_after_being_preyed() const;
	virtual void on_tick();
};

#endif