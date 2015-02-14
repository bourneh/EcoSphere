#include "Vector2D.h"
#include "Entity.h"
/*
 *消费者。主要为动物。
 *这里实现的动物的行为有捕食、躲避捕食者、随机运动。
*/
#ifndef DS_ECOSYSTEM_COMSUMER
#define DS_ECOSYSTEM_COMSUMER

class Consumer : public Entity
{
public:
	virtual bool is_producer() const;
	virtual bool is_consumer() const;

	double	get_speed() const;
	void	set_speed(double speed);
	
	//reactions
	virtual void on_eaten();

	//behaviors
	virtual bool predate(double speed);//捕食
	virtual bool brownian_motion(double speed);//布朗运动
	virtual bool avoid_predator(double speed);//躲避捕食者
	virtual void multiply();//繁殖
protected:
	Consumer(EcoSystem *eco_system);
	struct avoid_predator_str
	{
		int t;
		Vector2D v;
		bool b;
		avoid_predator_str() : t(6) {}
	}aps;
private:
	double speed;
};

#endif // DS_ECOSYSTEM_COMSUMER
