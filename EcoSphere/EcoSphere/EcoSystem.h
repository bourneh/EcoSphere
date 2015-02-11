#include <map>
#include <set>
#include <list>
#include <vector>
#include <mutex>
#include <string>
#include <random>
#include <queue>
#include <functional>
/*
 *EcoSystem�ࡣ���ڿ���������̬ϵͳ�����С�
*/


#ifndef DS_ECOSYSTEM
#define DS_ECOSYSTEM

#include "Animation.h"
#include "AnimationDisplayWindow.h"
#include "Timer.h"
#include "Vector2D.h"

class EcoSystemAnimation;
class EcoSystemTimerTask;

class Environment;
class FoodWeb;
class Entity;
class Consumer;

class EcoSystem
{
	friend class EcoSystemAnimation;//������ʾ��̬ϵͳ����
	friend class EcoSystemTimerTask;//��ʱ��
public:
	const static unsigned int	DEFAULT_WIDTH							= 1200;	//Ĭ�ϳ��Ϳ�
	const static unsigned int	DEFAULT_HEIGHT							= 700;
	const static unsigned int	CHUNK_SIZE								= 32;   //���ڷ�Χ�ǳ��������������̬ϵͳ�ֳ�ÿ���СΪ32*32�Ŀ�������
	static Vector2D random_position();//�������һ������̬ϵͳ�ڵ�λ������
	static double random_double();//�������һ����Χ��[0, 1]��doubleֵ
	static double random_angle();//�������һ����Χ��[0, 360]��doubleֵ
	static void prevent_overstep(Vector2D &position); 
	static void prevent_overstep(Entity *entity);//�������������ڷ�ֹ�����λ�ó�����̬ϵͳ�ķ�Χ
	EcoSystem();
	//virtual ~EcoSystem();

	void run();//����
	void pause();//��ͣ
	void resume();//��������
	void reset();//���ã�δд�ã�
	virtual void on_tick();//ÿtick����һ���������
	FoodWeb*		get_food_web_instance();//ʳ����
	void			set_food_web(FoodWeb *food_web);
	Environment*	get_environment_instance();//����
	void			set_environment(Environment *environment);

	//void register_species(Entity *species);
	void	spawn_entity(Entity *entity);//����̬ϵͳ�����λ����������entity���ڲ������������Ǹ�����
	void	spawn_entity(Entity *entity, Vector2D position);//����̬ϵͳ���ض�λ����������entity
	bool	try_eat(Entity *predator, Entity *prey);//predator���Բ�ʳprey�������뿴ʵ��
	Entity *find_entity(Entity *source, std::set<std::string> &types);//��������source���ڿ�Ϊ���ĵ�5*5�Ŀ����������һ��������types������
	std::set<Entity *> find_all_entity(Entity *source, std::set<std::string> &types);//��������source���ڿ�Ϊ���ĵ�5*5�Ŀ��в�������������types������
	Entity *find_prey(Entity *source);////��������source���ڿ�Ϊ���ĵ�5*5�Ŀ����������һ��source��ʳ�
private:
	Entity *find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);//��һ�������������һ��������types������
	std::set<Entity*> find_all_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);//��һ�����������������������types������
	std::map<std::string, Entity*>         species_list;//���������б�
	std::vector<Entity*>                    entities[50][50];//��
	std::list<Entity*>						update_list;//���ŵ������б�ÿ��tick����һ��
	std::vector<Entity*>					dead_entities;//�����������б���EcoSystem�౻����ʱ����ռ�õ��ڴ�Żᱻ�ͷ�
	FoodWeb     *food_web;
	Environment *environment;

	AnimationDisplayWindow	*eco_system_window;
	EcoSystemAnimation		*eco_system_animation;
	EcoSystemTimerTask		*eco_system_timer_task;
	Timer					*eco_system_timer;


	std::mutex mtx;
	bool paused;
};

class EcoSystemAnimation : public Animation
{
public:
	static bool render_order(const Entity *a, const Entity *b);
	EcoSystemAnimation(EcoSystem *eco_system);
	virtual void render(Gdiplus::Graphics *g);
private:
	EcoSystem *eco_system;
};
class EcoSystemTimerTask : public TimerTask
{
public:
	EcoSystemTimerTask(EcoSystem *eco_system);
	virtual void task();

private:
	EcoSystem *eco_system;
};
#endif // DS_ECOSYSTEM