#include "Consumer.h"

#ifndef DS_ECOSYSTEM_BASIC_PLANT_EATING_CONSUMER
#define DS_ECOSYSTEM_BASIC_PLANT_EATING_CONSUMER

class DemoCow : public Consumer
{
public:
	DemoCow(EcoSystem *eco_system);
	virtual ~DemoCow();

	virtual std::string get_species_name() const;
	virtual Gdiplus::Image* get_entity_image() const;
	virtual Entity* new_entity() const;

	virtual void on_tick();
};

#endif
