/*
 *生态系统的组成部分包括非生物的物质和能量、生产者、消费者、分解者等。
 *这里用一个基类EcoSystemObject来表示。DynamicSystemObject派生自EcoSystemObject，用于表示动态的生态系统成分。
 *为了简化代码，食物网（FoodWeb类）也派生自EcoSystemObject。
 *环境、生物等动态成分派生自DynamicEcoSystemObject，食物网派生自EcoSystemObject。
*/

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