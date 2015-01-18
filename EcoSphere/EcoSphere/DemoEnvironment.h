#include "Environment.h"

#ifndef DS_ECOSYSTEM_DEMO_ENVIRONMENT
#define DS_ECOSYSTEM_DEMO_ENVIRONMENT

class DemoEnvironment : public Environment
{
public:
	DemoEnvironment(EcoSystem *eco_system);
	virtual void on_tick();
	virtual void render_environment_background(Gdiplus::Graphics *g) const;
};

#endif