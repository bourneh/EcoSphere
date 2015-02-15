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
 *EcoSystem class.
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
	friend class EcoSystemAnimation;//Used to display animations of this ecosystem.
	friend class EcoSystemTimerTask;//The timer to trigger this ecosystem.
public:
	const static unsigned int	DEFAULT_WIDTH							= 1200;	//Default width and height.
	const static unsigned int	DEFAULT_HEIGHT							= 700;

	/*
	 *There are 1200 * 700 points inside this ecosystem. It takes very large amount of time to iterate it over.
	 *So here separates the whole ecosystem into several chunks with each chunk's size 32 * 32.
	 */
	const static unsigned int	CHUNK_SIZE								= 32;


	static Vector2D random_position();//Generate a random position vector. The position is guaranteed to be inside the ecosystem.
	static double random_double();//Generate a random double value with the range ------!!!!!!!------[0, 1]------!!!!!!------.
	static double random_angle();//Generate a random double value with the range ------!!!!!!!------[0, 360]------!!!!!!------.

	//These two functions are used to prevent the positions of the entities from being outside the ecosystem.
	static void prevent_overstep(Vector2D &position); 
	static void prevent_overstep(Entity *entity);


	EcoSystem();
	//virtual ~EcoSystem();

	void run();
	void pause();
	void resume();
	void reset();

	//on_tick() function is called by a timer.
	virtual void on_tick();


	FoodWeb*		get_food_web_instance();
	void			set_food_web(FoodWeb *food_web);
	Environment*	get_environment_instance();
	void			set_environment(Environment *environment);

	//Register a species to ecosystem.
	void	register_species(Entity *species);

	//Spawn an entity at a random position
	void	spawn_entity(Entity *entity);

	//Spawn an entity at a specified position
	void	spawn_entity(Entity *entity, Vector2D position);

	//The predator tries to predate its prey.
	bool	try_eat(Entity *predator, Entity *prey);

	//在以生物source所在chunk为中心的5*5的chunk中随机查找一个类型在types里的生物。
	Entity *find_entity(Entity *source, std::set<std::string> &types);

	//在以生物source所在chunk为中心的5*5的chunk中随机查找一个source的食物。
	Entity *find_prey(Entity *source);

	//统计以生物source所在块为中心的5*5的chunk中source的捕食者的数量。
	int get_around_predators_count(Entity* source);

	//统计以生物source所在块为中心的5*5的chunk中source的捕食者的位置向量的和
	//在Consumer的avoid_predator()函数中有说明这个位置向量和的用途
	Vector2D get_around_predators_position_sum(Entity* source);
private:
	
	//获得特定chunk中在types里的生物的数量。
	int get_chunk_entity_count(int chunk_r, int chunk_c, std::set<std::string> &types);

	//获得特定chunk中所有在types里的生物的位置向量和。
	Vector2D get_chunk_entity_position_sum(int chunk_r, int chunk_c, std::set<std::string> &types);
	
	//在一个chunk中随机查找一个类型在types里的生物。
	Entity *find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);

	std::map<std::string, Entity*>         species_list;//生物种类列表
	std::list<Entity*>						update_list;//活着的生物列表，每个tick更新一次
	std::vector<Entity*>					dead_entities;//死掉的生物列表，在EcoSystem类被销毁时它们占用的内存才会被释放


	/*
	 *chunk.
	 *每个chunk维护这个chunk里的各种生物的数量和它们的位置向量和
	*/
	struct chunk_structure
	{
		std::map<std::string, Vector2D> position_sum;
		std::map<std::string, int> count;
		std::set<Entity*>entities;
	}chunk[50][50];


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