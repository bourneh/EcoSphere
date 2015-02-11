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

EcoSystemAnimation::EcoSystemAnimation(EcoSystem *eco_system) :
Animation(EcoSystem::DEFAULT_WIDTH, EcoSystem::DEFAULT_HEIGHT)
{
	this->eco_system = eco_system;
}

void EcoSystemAnimation::render(Gdiplus::Graphics *g)
{
	using namespace Gdiplus;


	eco_system->mtx.lock();
	rendering_mutex.lock();
	eco_system->environment->render_environment_background(g);
	std::vector<Entity*>::iterator it;
	for (int r = 0; r < (EcoSystem::DEFAULT_WIDTH + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; r++)
		for (int c = 0; c < (EcoSystem::DEFAULT_HEIGHT + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; c++)
		{
			std::vector<Entity*> &ents = eco_system->entities[r][c];
			//std::sort(ents.begin(), ents.end(), EcoSystemAnimation::render_order);
			for (it = ents.begin(); it != ents.end(); it++)
			{
				Entity &ent = *(*it);
				g->DrawImage(ent.get_entity_image(), (int)ent.get_position().x, (int)ent.get_position().y);
			}
		}
	eco_system->environment->render_environment_effects(g);

	rendering_mutex.unlock();
	eco_system->mtx.unlock();
}

bool EcoSystemAnimation::render_order(const Entity *a, const Entity *b)
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
	eco_system_timer_task = new EcoSystemTimerTask(this);
	eco_system_timer = new Timer(100, *eco_system_timer_task);
	eco_system_animation = new EcoSystemAnimation(this);
	eco_system_window = new AnimationDisplayWindow(L"EcoSystem", DEFAULT_WIDTH, DEFAULT_HEIGHT, eco_system_animation);
	eco_system_window->show();
	paused = false;
}

Vector2D EcoSystem::random_position()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> posx(0.0, (double)DEFAULT_WIDTH);
	static std::uniform_real_distribution<double> posy(0.0, (double)DEFAULT_HEIGHT);
	double x = posx(generator), y = posy(generator);
	return Vector2D(x, y);
}

void EcoSystem::prevent_overstep(Vector2D &position)
{
	if (position.x < 0.01)
		position.x = 0.1;
	if (position.x > DEFAULT_WIDTH - 0.01)
		position.x = DEFAULT_WIDTH - 0.1;
	if (position.y < 0.01)
		position.y = 0.1;
	if (position.y > DEFAULT_HEIGHT - 0.01)
		position.y = DEFAULT_HEIGHT - 0.1;
}

void EcoSystem::prevent_overstep(Entity *entity)
{
	Vector2D position = entity->get_position();
	prevent_overstep(position);
	entity->set_position(position);
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
	std::list<Entity*>::iterator list_it;
	for (list_it = update_list.begin(); list_it != update_list.end();++list_it)
	{
		if ((*list_it)->is_alive())
		{
			(*list_it)->on_tick();
			prevent_overstep(*list_it);
			if ((*list_it)->get_energy() < 0.0)
				(*list_it)->set_dead();
		}
	}
	for (list_it = update_list.begin(); list_it != update_list.end();)
	{
		if (!((*list_it)->is_alive()))
			list_it = update_list.erase(list_it);
		else
			++list_it;
	}
	for (int r = 0; r < (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE; r++)
	{
		for (int c = 0; c < (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE; c++)
		{
			std::vector<Entity*> &ents = entities[r][c];
			for (it = ents.begin(); it != ents.end();)
			{
				Entity &ent = *(*it);
				if (!ent.is_alive())
				{
					delete *it;
					it = ents.erase(it);
					continue;
				}
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
	prevent_overstep(position);
	entity->set_position(position);
	int chunk_r = (int)entity->get_position().x / CHUNK_SIZE, chunk_c = (int)entity->get_position().y / CHUNK_SIZE;
	entities[chunk_r][chunk_c].push_back(entity);
	update_list.push_back(entity);
}

void EcoSystem::spawn_entity(Entity *entity)
{
	spawn_entity(entity, random_position());
}

void EcoSystem::run()
{
	eco_system_timer->start();
	eco_system_window->start();
}

void EcoSystem::pause()
{
	mtx.lock();
	paused = true;
}
void EcoSystem::resume()
{
	if (paused)
	{
		mtx.unlock();
		paused = false;
	}
}
void reset()
{

}


bool EcoSystem::try_eat(Entity *predator, Entity *prey)
{
	const static double MIN_EATABLE_DISTANCE = 10.0;
	if ((food_web->is_prey(prey, predator))
		&& ((predator->get_position() - prey->get_position()).modulus() < MIN_EATABLE_DISTANCE)
		&& (predator->is_alive())
		&& (prey->is_alive()))
	{
		predator->set_energy(predator->get_energy() - prey->get_cost_of_being_preyed());
		if (predator->get_energy() < 0.0)
			predator->set_dead();
		else
			predator->set_energy(predator->get_energy() + prey->get_gain_after_being_preyed());
		prey->set_dead();
		return true;
	}
	return false;
}

Entity *EcoSystem::find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c)
{
	if (chunk_r < 0 || chunk_c < 0 || chunk_r >= (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE || chunk_c >= (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE)
		return NULL;
	std::vector<Entity*> &ents = entities[chunk_r][chunk_c];
	std::vector<Entity*>::iterator it;
	std::vector<Entity*> entity_founded;
	for (it = ents.begin(); it != ents.end(); it++)
	{
		if (types.find((*it)->get_species_name()) != types.end())
			entity_founded.push_back(*it);
	}
	if (entity_founded.empty())
		return NULL;
	else
		return entity_founded[(unsigned int)(EcoSystem::random_double() * entity_founded.size())];
}

Entity *EcoSystem::find_entity(Entity *source, std::set<std::string> &types)
{
	int source_chunk_r = (int)source->get_position().x / CHUNK_SIZE, source_chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	Entity *tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r, source_chunk_c))			!= NULL) return tmp;
										 
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r, source_chunk_c - 1))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r, source_chunk_c + 1))		!= NULL) return tmp;
										 
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c - 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c + 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c - 1))	!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c + 1))	!= NULL) return tmp;
										 
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r, source_chunk_c - 2))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c))		!= NULL) return tmp;
	if ( (tmp = find_entity_in_chunk(types, source_chunk_r, source_chunk_c + 2))		!= NULL) return tmp;

	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c - 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c + 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c - 1)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c + 1)) != NULL) return tmp;
										
	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c + 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c - 2)) != NULL) return tmp;
	if ((tmp = find_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c + 2)) != NULL) return tmp;
	return NULL;
}

Entity *EcoSystem::find_prey(Entity *source)
{
	if (source->get_target() == NULL)
	{
		Entity *tmp = find_entity(source, food_web->get_prey_set(source));
		source->set_target(tmp);
		if (tmp != NULL)
			tmp->add_ts(source);
	}

	return source->get_target();	
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
