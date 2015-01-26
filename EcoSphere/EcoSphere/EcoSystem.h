#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <random>
#include <queue>
#include <functional>
#ifndef DS_ECOSYSTEM
#define DS_ECOSYSTEM

#include "Animation.h"
#include "DisplayWindow.h"
#include "Timer.h"
#include "Vector2D.h"

class EcoSystemRenderer;
class EcoSystemTimerTask;

class Environment;
class FoodWeb;
class Entity;
class Consumer;

class EcoSystem
{
	friend class EcoSystemRenderer;
	friend class EcoSystemTimerTask;
public:
	const static unsigned int	DEFAULT_WIDTH							= 1200;
	const static unsigned int	DEFAULT_HEIGHT							= 700;
	const static unsigned int	CHUNK_SIZE								= 32;
	static Vector2D random_position();
	static double random_double();
	static double random_angle();
	static void prevent_overstep(Vector2D &position);
	EcoSystem();
	//virtual ~EcoSystem();

	void run();
	void pause();
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
	void	fight(Consumer *a, Consumer *b);
	Entity *find_entity(Entity *source, std::string type);
	Entity *find_prey(Entity *source);
private:
	std::map<std::string, Entity*>         species_list;
	std::vector<Entity*>                    entities[50][50];
	std::queue<Entity*>						entities_about_to_spawn;
	FoodWeb     *food_web;
	Environment *environment;

	DisplayWindow       *eco_system_display_window;
	Animation           *eco_system_animation;
	Timer               *eco_system_timer;

	EcoSystemRenderer   *eco_system_renderer;
	EcoSystemTimerTask  *eco_system_timer_task;


	std::mutex mtx;

	Entity *find_entity_in_chunk(std::string type, int chunk_r, int chunk_c);
};

class EcoSystemRenderer : public RenderTask
{
public:
	static bool render_order(const Entity *a, const Entity *b);
	EcoSystemRenderer(EcoSystem *eco_system);
	virtual Gdiplus::Image *render();
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