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

EcoSystemAnimation::EcoSystemAnimation(EcoSystem *eco_system) :
Animation(EcoSystem::DEFAULT_WIDTH + 35, EcoSystem::DEFAULT_HEIGHT + 58)
{
	this->eco_system = eco_system;
}

void EcoSystemAnimation::render(Gdiplus::Graphics *g)//渲染
{
	using namespace Gdiplus;


	eco_system->mtx.lock();
	rendering_mutex.lock();


	/*
	 *渲染顺序为先渲染环境背景，在渲染各种生物，最后渲染环境的其他效果。
	*/
	eco_system->environment->render_environment_background(g);//渲染环境背景

	//==================渲染生物=============================================================================
	std::list<Entity*>::iterator it;
	for (it = eco_system->update_list.begin(); it != eco_system->update_list.end(); it++)
	{
		Entity &ent = *(*it);
		g->DrawImage(ent.get_entity_image(), (int)ent.get_position().x, (int)ent.get_position().y);
	}
	//======================================================================================================
	eco_system->environment->render_environment_effects(g);//渲染环境的其他效果

	rendering_mutex.unlock();
	eco_system->mtx.unlock();
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
	eco_system_window = new AnimationDisplayWindow(L"EcoSystem", DEFAULT_WIDTH + 35, DEFAULT_HEIGHT + 58, eco_system_animation);
	eco_system_window->show();
	paused = false;
}

//Generate a random position vector. The position is guaranteed to be inside the ecosystem.
Vector2D EcoSystem::random_position()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> posx(0.0, (double)DEFAULT_WIDTH);
	static std::uniform_real_distribution<double> posy(0.0, (double)DEFAULT_HEIGHT);
	double x = posx(generator), y = posy(generator);
	return Vector2D(x, y);
}

//Generate a random double value with the range ------!!!!!!!------[0, 1]------!!!!!!------.
double EcoSystem::random_double()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> rand_double(0.0, 1.0);
	return rand_double(generator);
}

//Generate a random double value with the range ------!!!!!!!------[0, 360]------!!!!!!------.
double EcoSystem::random_angle()
{
	static std::default_random_engine generator((unsigned int)time(NULL));
	static std::uniform_real_distribution<double> rand_double(0.0, 360.0);
	return rand_double(generator);
}

//Prevent the positions from being outside the ecosystem
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

//Prevent the positions of the entities from being outside the ecosystem
void EcoSystem::prevent_overstep(Entity *entity)
{
	Vector2D position = entity->get_position();
	prevent_overstep(position);
	entity->set_position(position);
}

void EcoSystem::on_tick()//！！！！！！！生态系统的on_tick函数
{
	mtx.lock();//由于渲染在另一个线程，而且和当前线程有共享资源，所以要加互斥锁。
	environment->on_tick();//更新环境
	std::vector<Entity*>::iterator it;
	std::list<Entity*>::iterator list_it;

	//=========================更新生物====================================
	for (list_it = update_list.begin(); list_it != update_list.end();)
	{
		if ((*list_it)->is_alive())//如果这个生物活着
		{
			/*
			 *更新生物涉及的操作比较多，包括以下几个
			 *更新生物所在chunk的生物数量
			 *更新生物所在chunk的位置向量和
			 *如果生物从一个chunk跑到了另一个chunk，还要更新两个chunk的东西
			*/
			Vector2D prev_pos = (*list_it)->get_position();
			int prev_chunk_r = (int)((*list_it)->get_position().x / CHUNK_SIZE);
			int prev_chunk_c = (int)((*list_it)->get_position().y / CHUNK_SIZE);
			chunk[prev_chunk_r][prev_chunk_c].position_sum[(*list_it)->get_species_name()] -= prev_pos;//更新位置和
			(*list_it)->on_tick();
			prevent_overstep(*list_it);//防越界
			int current_chunk_r = (int)((*list_it)->get_position().x / CHUNK_SIZE);
			int current_chunk_c = (int)((*list_it)->get_position().y / CHUNK_SIZE);
			chunk[current_chunk_r][current_chunk_c].position_sum[(*list_it)->get_species_name()] += (*list_it)->get_position();//还是更新位置和

			//如果HP小于0,说明这个生物死掉了
			if ((*list_it)->get_hp() < 0.0)
				(*list_it)->set_dead();

			//如果生物从一个chunk移动到了另一个chunk，要更新chunk的生物数量和生物列表
			if (prev_chunk_r != current_chunk_r || prev_chunk_c != current_chunk_c)
			{
				chunk[prev_chunk_r][prev_chunk_c].entities.erase(*list_it);
				chunk[prev_chunk_r][prev_chunk_c].count[(*list_it)->get_species_name()]--;
				
				chunk[current_chunk_r][current_chunk_c].entities.insert(*list_it);
				chunk[current_chunk_r][current_chunk_c].count[(*list_it)->get_species_name()]++;
			}

			++list_it;
		}
		else//如果这个生物死了
		{
			int r = (int)((*list_it)->get_position().x / CHUNK_SIZE);
			int c = (int)((*list_it)->get_position().y / CHUNK_SIZE);
			chunk[r][c].entities.erase(*list_it);
			chunk[r][c].count[(*list_it)->get_species_name()]--;
			chunk[r][c].position_sum[(*list_it)->get_species_name()] -= (*list_it)->get_position();
			dead_entities.push_back(*list_it);
			list_it = update_list.erase(list_it);
		}
	}

	//=========================更新生物结束===================================
	mtx.unlock();
}

//Spawn an entity at a specified position
void EcoSystem::spawn_entity(Entity *entity, Vector2D position)
{
	prevent_overstep(position);
	entity->set_position(position);
	int chunk_r = (int)entity->get_position().x / CHUNK_SIZE, chunk_c = (int)entity->get_position().y / CHUNK_SIZE;
	
	chunk[chunk_r][chunk_c].entities.insert(entity);
	chunk[chunk_r][chunk_c].position_sum[entity->get_species_name()] += entity->get_position();
	chunk[chunk_r][chunk_c].count[entity->get_species_name()]++;

	update_list.push_back(entity);
}

//Spawn an entity at a random position
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


bool EcoSystem::try_eat(Entity *predator, Entity *prey)//尝试捕食
{
	const static double MIN_EATABLE_DISTANCE = 10.0;//两个生物间的距离小于10才能捕食
	if ((food_web->is_prey(prey, predator)) //prey是否是predator的食物
		&& ((predator->get_position() - prey->get_position()).modulus() < MIN_EATABLE_DISTANCE) //距离是否小于10
		&& (predator->is_alive())
		&& (prey->is_alive()))//两个生物是否活着
	{
		predator->set_hp(predator->get_hp() - prey->get_cost_of_being_preyed());
		if (predator->get_hp() < 0.0)
			predator->set_dead();
		else
			predator->set_hp(predator->get_hp() + prey->get_gain_after_being_preyed());
		prey->set_dead();
		return true;
	}
	return false;
}

Entity *EcoSystem::find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c)//在一个特定块中查找类型在type里的生物
{
	//防止越界
	if (chunk_r < 0 || chunk_c < 0 || chunk_r >= (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE || chunk_c >= (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE)
		return NULL;

	std::set<Entity*> &ents = chunk[chunk_r][chunk_c].entities;
	std::set<Entity*>::iterator it;
	std::vector<Entity*> entity_founded;
	
	//把符合条件的生物加入entity_founded
	for (it = ents.begin(); it != ents.end(); it++)
	{
		if (types.find((*it)->get_species_name()) != types.end())
			entity_founded.push_back(*it);
	}
	if (entity_founded.empty())
		return NULL;
	else
		return entity_founded[(unsigned int)(EcoSystem::random_double() * entity_founded.size())];//返回随机一个
		
}

//获得特定chunk中在types里的生物的数量。
int EcoSystem::get_chunk_entity_count(int chunk_r, int chunk_c, std::set<std::string> &types)
{
	std::set<std::string>::iterator it;
	int sum = 0;
	for (it = types.begin(); it != types.end(); it++)
		sum += chunk[chunk_r][chunk_c].count[*it];
	return sum;
}

//获得特定chunk中所有在types里的生物的位置向量和。
Vector2D EcoSystem::get_chunk_entity_position_sum(int chunk_r, int chunk_c, std::set<std::string> &types)
{
	std::set<std::string>::iterator it;
	Vector2D sum;
	for (it = types.begin(); it != types.end(); it++)
		sum += chunk[chunk_r][chunk_c].position_sum[*it];
	return sum;
}

//在以生物source所在chunk为中心的5*5的chunk中随机查找一个类型在types里的生物。
Entity *EcoSystem::find_entity(Entity *source, std::set<std::string> &types)
{
	int source_chunk_r = (int)source->get_position().x / CHUNK_SIZE, source_chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	Entity *tmp;

	//在以当前生物所在的块为中心的5*5的块中查找生物。块的查找顺序大致为从近到远
	//以下数字为chunk查找的顺序。0表示生物所在的chunk

	//     [21] [13]  [9 ]  [14]  [22]
	//     [17] [5 ]  [1 ]  [6 ]  [18]
	//     [10] [3 ]  [0 ]  [4 ]  [12]
	//     [15] [7 ]  [2 ]  [8 ]  [16]
	//     [23] [19]  [11]  [20]  [24]
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
	
	//没找到就返回NULL
	return NULL;
}

//在以生物source所在chunk为中心的5*5的chunk中随机查找一个source的食物。
Entity *EcoSystem::find_prey(Entity *source)
{
	if (!(source->get_target() != NULL && source->get_target()->is_alive()))
	{
		Entity *tmp = find_entity(source, food_web->get_prey_set(source));
		source->set_target(tmp);
		if (tmp != NULL)
			tmp->add_predator(source);
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


//Register a species to ecosystem.
void EcoSystem::register_species(Entity* species)
{
	if (species_list.find(species->get_species_name()) != species_list.end())
		return;

	species_list[species->get_species_name()] = species;

	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
		{
			chunk[i][j].position_sum[species->get_species_name()] = Vector2D(0, 0);
			chunk[i][j].count[species->get_species_name()] = 0;
		}
}

//统计以生物source所在块为中心的5*5的chunk中source的捕食者的数量。
int EcoSystem::get_around_predators_count(Entity* source)
{
	int chunk_r = (int)source->get_position().x / CHUNK_SIZE, chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	int r1 = (chunk_r - 2) < 0 ? 0 : chunk_r - 2;
	int r2 = (chunk_r + 2) >= 50 ? 49 : chunk_r + 2;
	int c1 = (chunk_c - 2) < 0 ? 0 : chunk_c - 2;
	int c2 = (chunk_c + 2) >= 50 ? 49 : chunk_c + 2;

	std::set<std::string> &predators = food_web->get_predator_set(source);
	std::set<std::string>::iterator it;
	int sum = 0;

	for (int r = r1; r <= r2; r++)
		for (int c = c1; c != c2; c++)
			for (it = predators.begin(); it != predators.end(); it++)
				sum += chunk[r][c].count[*it];

	return sum;
}

//统计以生物source所在块为中心的5*5的chunk中source的捕食者的位置向量的和
//在Consumer的avoid_predator()函数中有说明这个位置向量和的用途
Vector2D EcoSystem::get_around_predators_position_sum(Entity* source)
{
	int chunk_r = (int)source->get_position().x / CHUNK_SIZE, chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	int r1 = (chunk_r - 2) < 0 ? 0 : chunk_r - 2;
	int r2 = (chunk_r + 2) >= 50 ? 49 : chunk_r + 2;
	int c1 = (chunk_c - 2) < 0 ? 0 : chunk_c - 2;
	int c2 = (chunk_c + 2) >= 50 ? 49 : chunk_c + 2;

	std::set<std::string> &predators = food_web->get_predator_set(source);
	std::set<std::string>::iterator it;
	Vector2D sum = 0;

	for (int r = r1; r <= r2; r++)
		for (int c = c1; c != c2; c++)
			for (it = predators.begin(); it != predators.end(); it++)
				sum += chunk[r][c].position_sum[*it];

	return sum;
}