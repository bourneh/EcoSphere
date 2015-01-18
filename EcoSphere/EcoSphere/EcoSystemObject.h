#ifndef DS_ECOSYSTEM_ECOSYSTEMOBJECT
#define DS_ECOSYSTEM_ECOSYSTEMOBJECT

class EcoSystem;

class EcoSystemObject
{
public:
	EcoSystem *get_eco_system_instance() const
	{
		return eco_system;
	}
protected:
	EcoSystemObject(EcoSystem *eco_system)
	{
		this->eco_system = eco_system;
	}
	EcoSystem *eco_system;


};

class DynamicEcoSystemObject : public EcoSystemObject
{
public:
	virtual void on_tick() = 0;
protected:
	DynamicEcoSystemObject(EcoSystem *eco_system) :
		EcoSystemObject(eco_system) {}
};
#endif