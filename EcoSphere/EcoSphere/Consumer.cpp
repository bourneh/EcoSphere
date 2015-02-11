#include "Consumer.h"
#include "EcoSystem.h"
#include "FoodWeb.h"
Consumer::Consumer(EcoSystem *eco_system) :
Entity(eco_system) {}


bool Consumer::is_producer() const
{
	return false;
}

bool Consumer::is_consumer() const
{
	return true;
}



double Consumer::get_speed() const
{
	return speed;
}

void Consumer::set_speed(double speed)
{
	this->speed = speed;
}

//捕食。参数speed是捕食时的速度。
bool Consumer::predate(double speed)
{
	set_speed(speed);
	Entity *food = eco_system->find_prey(this); //寻找食物
	if (food != NULL)
	{
		if (!eco_system->try_eat(this, food))
		{
			//计算过程参考附图
			Vector2D target_position = food->get_position();
			Vector2D distance = target_position - this->get_position();
			Vector2D displacement = distance * (1.0 / distance.modulus()) * get_speed();
			//if (displacement.modulus() < get_speed())
			target_position = get_position() + displacement;
			this->set_position(target_position);
		}
		return true;
	}
	return false;
}

//向随机方向运动。这里称为布朗运动。speed是运动的速度。
bool Consumer::brownian_motion(double speed)
{
	set_speed(speed);
	Vector2D unit_vector(1.0, 0.0);//单位向量
	unit_vector = unit_vector.rotate(EcoSystem::random_angle());//旋转随机的角度
	Vector2D displacement = unit_vector * get_speed();//speed*旋转后的单位向量
	Vector2D target_position = get_position() + displacement;
	EcoSystem::prevent_overstep(target_position);
	set_position(target_position);

	//看不懂代码请参考附图
	return true;
}

//躲避捕食者
bool Consumer::avoid_predator(double speed)
{
	//过程请参考附图
	if (aps.t <= 5)
	{
		++aps.t;
		set_position(get_position() + aps.v);
		return true;
	}
	std::set<Entity*> predators = eco_system->find_all_entity(this, eco_system->get_food_web_instance()->get_predator_set(this));
	if (!predators.empty())
	{
		aps.t = 0;
		set_speed(speed);
		Vector2D d;
		for (std::set<Entity*>::iterator it = predators.begin(); it != predators.end(); it++)
			d += get_position() - (*it)->get_position();
		if (d.modulus() < 1.0)
			d = Vector2D(1, 0).rotate(EcoSystem::random_angle()) * get_speed();
		else
			d = d * (get_speed() / d.modulus());
		set_position(get_position() + d);
		aps.v = d;
		return true;
	}
	return false;
}

void Consumer::multiply()//繁殖
{
	eco_system->spawn_entity(new_entity());
}
void Consumer::on_eaten()//生物被吃掉的时候会调用这个函数。如果想实现被吃掉时通知附近的朋友等等的功能可以重载这个函数。
{}
