#include "ecosystem_gdiplus.h"
#include "EcoSystem.h"
#include "Entity.h"
#include "Environment.h"
#include "FoodWeb.h"
#include "DisplayWindow.h"
#include "Animation.h"
#include "Vector2D.h"
#include "Consumer.h"
#include "DemoGrass.h"
#include <random>
#include <set>
#include <algorithm>
#include <queue>

const static double	ENERGY_SOIL_FERTILITY_CONVERSION_RATE = 0.20;

EcoSystemRenderer::EcoSystemRenderer(EcoSystem *eco_system) :
RenderTask(EcoSystem::DEFAULT_WIDTH, EcoSystem::DEFAULT_HEIGHT)
{
	this->eco_system = eco_system;
}

Gdiplus::Image *EcoSystemRenderer::render()
{
	using namespace Gdiplus;
	Graphics *g = get_graphics_instance();


	eco_system->mtx.lock();
	eco_system->environment->render_environment_background(g);
	std::vector<Entity*>::iterator it;
	for (int r = 0; r < (EcoSystem::DEFAULT_WIDTH + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; r++)
		for (int c = 0; c < (EcoSystem::DEFAULT_HEIGHT + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; c++)
		{
			std::vector<Entity*> &ents = eco_system->entities[r][c];
			std::sort(ents.begin(), ents.end(), EcoSystemRenderer::render_order);
			for (it = ents.begin(); it != ents.end(); it++)
			{
				Entity &ent = *(*it);
				g->DrawImage(ent.get_entity_image(), (int)ent.get_position().x, (int)ent.get_position().y);
			}
		}
	eco_system->environment->render_environment_effects(g);

	eco_system->mtx.unlock();
	return RenderTask::render();
}

bool EcoSystemRenderer::render_order(const Entity *a, const Entity *b)
{
	return a->get_position().y < b->get_position().y;
}

EcoSystemTimerTask::EcoSystemTimerTask(EcoSystem *eco_system)
{
	this->eco_system = eco_system;
}

void EcoSystemTimerTask::task()
{
	eco_system->on_tick();
}

EcoSystem::EcoSystem()
{
	eco_system_renderer = new EcoSystemRenderer(this);
	eco_system_timer_task = new EcoSystemTimerTask(this);
	eco_system_display_window = new DisplayWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	eco_system_display_window->display();
	Sleep(500);
	eco_system_timer = new Timer(100, *eco_system_timer_task);
	eco_system_animation = new Animation(eco_system_display_window->get_window_handle(), 11, *eco_system_renderer);
}

Vector2D EcoSystem::random_position()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> posx(0.0, (double)DEFAULT_WIDTH);
	static std::uniform_real_distribution<double> posy(0.0, (double)DEFAULT_HEIGHT);
	double x = posx(generator), y = posy(generator);
	return Vector2D(x, y);
}

double EcoSystem::random_double()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> rand_double(0.0, 1.0);
	return rand_double(generator);
}

double EcoSystem::random_angle()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> rand_double(0.0, 360.0);
	return rand_double(generator);
}

void EcoSystem::on_tick()
{
	mtx.lock();
	environment->on_tick();
	std::vector<Entity*>::iterator it;
	std::queue<Entity*> q;
	for (int r = 0; r < (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE; r++)
	{
		for (int c = 0; c < (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE; c++)
		{
			std::vector<Entity*> &ents = entities[r][c];
			for (it = ents.begin(); it != ents.end();)
			{
				if (!((*it)->is_valid()))
				{
					Entity *tmp = (*it);
					it = ents.erase(it);
					delete tmp;
				}
				else
				{
					//(*it)->on_tick();
					q.push(*it);
					it++;
				}
			}
		}
	}

	while (!q.empty())
	{
		q.front()->on_tick();
		q.pop();
	}
	for (int r = 0; r < (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE; r++)
	{
		for (int c = 0; c < (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE; c++)
		{
			std::vector<Entity*> &ents = entities[r][c];
			for (it = ents.begin(); it != ents.end();)
			{
				Entity &ent = *(*it);
				int chunk_r = (int)ent.get_position().x / CHUNK_SIZE, chunk_c = (int)ent.get_position().y / CHUNK_SIZE;
				if (chunk_r != r || chunk_c != c)
				{
					it = ents.erase(it);
					entities[chunk_r][chunk_c].push_back(&ent);
				}
				else
					it++;
			}
		}
	}
	mtx.unlock();
}

void EcoSystem::spawn_entity(Entity *entity, Vector2D position)
{
	if (position.x < 0.0)
		position.x = 0.0;
	if (position.x > (double)DEFAULT_WIDTH - 0.1)
		position.x = (double)DEFAULT_WIDTH - 0.1;
	if (position.y < 0.0)
		position.y = 0.0;
	if (position.y > (double)DEFAULT_HEIGHT - 0.1)
		position.y = (double)DEFAULT_HEIGHT - 0.1;
	entity->set_position(position);
	int chunk_r = (int)entity->get_position().x / CHUNK_SIZE, chunk_c = (int)entity->get_position().y / CHUNK_SIZE;
	entities[chunk_r][chunk_c].push_back(entity);
}

void EcoSystem::spawn_entity(Entity *entity)
{
	spawn_entity(entity, random_position());
}

void EcoSystem::run()
{
	if (!eco_system_timer->is_running())
		eco_system_timer->start();
	if (!eco_system_animation->is_running())
		eco_system_animation->start();
}

void EcoSystem::pause()
{
	if (eco_system_timer->is_running())
		eco_system_timer->stop();
}

void reset()
{

}


bool EcoSystem::try_eat(Entity *predator, Entity *prey)
{
	const static double MIN_EATABLE_DISTANCE = 10.0;
	const static double ENERGY_TRANSFER_RATE = 0.20;
	if ((food_web->is_prey(prey, predator))
		&& ((predator->get_position() - prey->get_position()).modulus() < MIN_EATABLE_DISTANCE)
		&& (predator->is_valid())
		&& (prey->is_valid()))
	{
		predator->set_energy(predator->get_energy() + prey->get_energy() * ENERGY_TRANSFER_RATE);
		prey->set_valid(false);
		return true;
	}
	return false;
}

void EcoSystem::fight(Consumer *a, Consumer *b)
{
	double bound = a->get_strength() / (a->get_strength() + b->get_strength());
	if (random_double() < bound)
	{
		b->set_valid(false);
		a->set_energy(a->get_energy() - random_double() * 0.3 * b->get_energy());
	}
	else
	{
		a->set_valid(false);
		b->set_energy(b->get_energy() - random_double() * 0.3 * a->get_energy());
	}
}

Entity *EcoSystem::find_entity_in_chunk(std::string type, int chunk_r, int chunk_c)
{
	if (chunk_r < 0 || chunk_c < 0 || chunk_r >= (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE || chunk_c >= (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE)
		return NULL;
	std::vector<Entity*> &ents = entities[chunk_r][chunk_c];
	std::vector<Entity*>::iterator it;
	for (it = ents.begin(); it != ents.end(); it++)
	{
		if ((*it)->get_species_name() == type)
			return (*it);
	}
	return NULL;
}

Entity *EcoSystem::find_entity(Entity *source, std::string type)
{
	int source_chunk_r = (int)source->get_position().x / CHUNK_SIZE, source_chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	Entity *tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r, source_chunk_c))			!= NULL) return tmp;
	
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r - 1, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r + 1, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r, source_chunk_c - 1))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r, source_chunk_c + 1))		!= NULL) return tmp;
	
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r - 1, source_chunk_c - 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r - 1, source_chunk_c + 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r + 1, source_chunk_c - 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r + 1, source_chunk_c + 1))	!= NULL) return tmp;
	
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r - 2, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r, source_chunk_c - 2))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r + 2, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(type, source_chunk_r, source_chunk_c + 2))		!= NULL) return tmp;

	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 2, source_chunk_c - 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 2, source_chunk_c + 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 1, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 1, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 1, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 1, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 2, source_chunk_c - 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 2, source_chunk_c + 1)) != NULL) return tmp;

	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 2, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r - 2, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 2, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(type, source_chunk_r + 2, source_chunk_c + 2)) != NULL) return tmp;
	return NULL;
}

Entity *EcoSystem::find_prey(Entity *source)
{
	using namespace std;
	set<string> &s = food_web->get_prey_set(source);
	set<string>::iterator it;
	Entity *tmp;
	for (it = s.begin(); it != s.end(); it++)
	{
		if ((tmp = find_entity(source, *it)) != NULL)
			return tmp;
	}
	return NULL;
}

FoodWeb *EcoSystem::get_food_web_instance()
{
	return food_web;
}

void EcoSystem::set_food_web(FoodWeb *food_web)
{
	this->food_web = food_web;
}

Environment *EcoSystem::get_environment_instance()
{
	return environment;
}

void EcoSystem::set_environment(Environment *environment)
{
	this->environment = environment;
}
