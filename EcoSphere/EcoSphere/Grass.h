#include "Producer.h"
#include "EcoSystem.h"
#include "ecosystem_gdiplus.h"

#ifndef DS_ECOSYSTEM_GRASS
#define DS_ECOSYSTEM_GRASS

class Grass : public Producer
{
public:
	Grass(EcoSystem *eco_system);
	virtual std::string get_species_name() const;
	virtual Gdiplus::Image* get_entity_image() const;
	virtual Entity* new_entity() const;
	virtual void on_tick();
private:
	double water_content;
};

#endif