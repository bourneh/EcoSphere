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
 *EcoSystem类。用于控制整个生态系统的运行。
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
	friend class EcoSystemAnimation;//用于显示生态系统动画
	friend class EcoSystemTimerTask;//定时器
public:
	const static unsigned int	DEFAULT_WIDTH							= 1200;	//默认长和宽
	const static unsigned int	DEFAULT_HEIGHT							= 700;
	const static unsigned int	CHUNK_SIZE								= 32;   //由于范围非常大，所以这里把生态系统分成每块大小为32*32的块来管理
	static Vector2D random_position();//随机产生一个在生态系统内的位置坐标
	static double random_double();//随机产生一个范围是[0, 1]的double值
	static double random_angle();//随机产生一个范围是[0, 360]的double值
	static void prevent_overstep(Vector2D &position); 
	static void prevent_overstep(Entity *entity);//这两个函数用于防止生物的位置超出生态系统的范围
	EcoSystem();
	//virtual ~EcoSystem();

	void run();//运行
	void pause();//暂停
	void resume();//继续运行
	void reset();//重置（未写好）
	virtual void on_tick();//每tick运行一次这个函数
	FoodWeb*		get_food_web_instance();//食物网
	void			set_food_web(FoodWeb *food_web);
	Environment*	get_environment_instance();//环境
	void			set_environment(Environment *environment);

	//void register_species(Entity *species);
	void	spawn_entity(Entity *entity);//在生态系统的随机位置生成生物entity，内部调用了下面那个函数
	void	spawn_entity(Entity *entity, Vector2D position);//在生态系统的特定位置生成生物entity
	bool	try_eat(Entity *predator, Entity *prey);//predator尝试捕食prey，具体请看实现
	Entity *find_entity(Entity *source, std::set<std::string> &types);//在以生物source所在块为中心的5*5的块中随机查找一个类型在types里的生物。
	std::set<Entity *> find_all_entity(Entity *source, std::set<std::string> &types);//在以生物source所在块为中心的5*5的块中查找所有类型在types里的生物。
	Entity *find_prey(Entity *source);////在以生物source所在块为中心的5*5的块中随机查找一个source的食物。
private:
	Entity *find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);//在一个块中随机查找一个类型在types里的生物。
	std::set<Entity*> find_all_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);//在一个块中随机查找所有类型在types里的生物。
	std::map<std::string, Entity*>         species_list;//生物种类列表
	std::vector<Entity*>                    entities[50][50];//块
	std::list<Entity*>						update_list;//活着的生物列表，每个tick更新一次
	std::vector<Entity*>					dead_entities;//死掉的生物列表，在EcoSystem类被销毁时它们占用的内存才会被释放
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