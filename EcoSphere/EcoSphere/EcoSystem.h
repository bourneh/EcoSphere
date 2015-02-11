#include <map>
#include <set>
#include <list>
#include <vector>
#include <mutex>
#include <string>
#include <random>
#include <queue>
#include <functional>
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
	friend class EcoSystemAnimation;
	friend class EcoSystemTimerTask;
public:
	const static unsigned int	DEFAULT_WIDTH							= 1200;
	const static unsigned int	DEFAULT_HEIGHT							= 700;
	const static unsigned int	CHUNK_SIZE								= 32;
	static Vector2D random_position();
	static double random_double();
	static double random_angle();
	static void prevent_overstep(Vector2D &position);
	static void prevent_overstep(Entity *entity);
	EcoSystem();
	//virtual ~EcoSystem();

	void run();
	void pause();
	void resume();
	void reset();
	virtual void on_tick();
	FoodWeb*		get_food_web_instance();
	void			set_food_web(FoodWeb *food_web);
	Environment*	get_environment_instance();
	void			set_environment(Environment *environment);

	//void register_species(Entity *species);
	void	spawn_entity(Entity *entity);
	void	spawn_entity(Entity *entity, Vector2D position);
	bool	try_eat(Entity *predator, Entity *prey);
	Entity *find_entity(Entity *source, std::set<std::string> &types);
	Entity *find_prey(Entity *source);
private:
	Entity *find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c);
	
	std::map<std::string, Entity*>         species_list;
	std::vector<Entity*>                    entities[50][50];
	std::list<Entity*>						update_list;
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