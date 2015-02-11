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

//��ʳ������speed�ǲ�ʳʱ���ٶȡ�
bool Consumer::predate(double speed)
{
	set_speed(speed);
	Entity *food = eco_system->find_prey(this); //Ѱ��ʳ��
	if (food != NULL)
	{
		if (!eco_system->try_eat(this, food))
		{
			//������̲ο���ͼ
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

//����������˶��������Ϊ�����˶���speed���˶����ٶȡ�
bool Consumer::brownian_motion(double speed)
{
	set_speed(speed);
	Vector2D unit_vector(1.0, 0.0);//��λ����
	unit_vector = unit_vector.rotate(EcoSystem::random_angle());//��ת����ĽǶ�
	Vector2D displacement = unit_vector * get_speed();//speed*��ת��ĵ�λ����
	Vector2D target_position = get_position() + displacement;
	EcoSystem::prevent_overstep(target_position);
	set_position(target_position);

	//������������ο���ͼ
	return true;
}

//��ܲ�ʳ��
bool Consumer::avoid_predator(double speed)
{
	//������ο���ͼ
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

void Consumer::multiply()//��ֳ
{
	eco_system->spawn_entity(new_entity());
}
void Consumer::on_eaten()//���ﱻ�Ե���ʱ��������������������ʵ�ֱ��Ե�ʱ֪ͨ���������ѵȵȵĹ��ܿ����������������
{}
