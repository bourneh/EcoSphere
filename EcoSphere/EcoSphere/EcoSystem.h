#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <random>
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
	static Vector2D random_position();
	static double random_double();
	EcoSystem();
	//virtual ~EcoSystem();

	void run();
	void pause();
	void reset();
	virtual void on_tick();
	const FoodWeb* get_food_web_instance();
	const Environment* get_environment_instance();
	//void register_species(Entity *species);
	void spawn_entity(Entity *entity);
	void spawn_entity(Entity *entity, Vector2D position);
	bool try_eat(Entity *predator, Entity *prey);
	void fight(Consumer *a, Consumer *b);
	Entity *find_entity(Entity *source, std::string type);
	Entity *find_prey(Entity *source);
private:
	std::map<std::string, Entity*>         species_list;
	std::vector<Entity*>                    entities[50][50];

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