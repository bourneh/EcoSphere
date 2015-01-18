#include "Producer.h"
#include "EcoSystem.h"
#include "ecosystem_gdiplus.h"
#ifndef DS_ECOSYSTEM_BASIC_PRODUCER
#define DS_ECOSYSTEM_BASIC_PRODUCER

class DemoGrass : public Producer
{
public:
	DemoGrass(EcoSystem *eco_system);
	virtual std::string get_species_name() const;
	virtual Gdiplus::Image* get_entity_image() const;
	virtual Entity* new_entity() const;
	virtual void on_tick();
private:
};


#endif