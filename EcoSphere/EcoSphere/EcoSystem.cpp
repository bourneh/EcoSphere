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
Animation(EcoSystem::DEFAULT_WIDTH + 35, EcoSystem::DEFAULT_HEIGHT + 58)
{
	this->eco_system = eco_system;
}

void EcoSystemAnimation::render(Gdiplus::Graphics *g)//��Ⱦ
{
	using namespace Gdiplus;


	eco_system->mtx.lock();
	rendering_mutex.lock();
	eco_system->environment->render_environment_background(g);//����Ⱦ��������
	std::list<Entity*>::iterator it;
	//����Ⱦ����
	/*for (int r = 0; r < (EcoSystem::DEFAULT_WIDTH + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; r++)
		for (int c = 0; c < (EcoSystem::DEFAULT_HEIGHT + EcoSystem::CHUNK_SIZE) / EcoSystem::CHUNK_SIZE; c++)
		{
			std::set<Entity*> &ents = eco_system->chunk[r][c].entities;
			//std::sort(ents.begin(), ents.end(), EcoSystemAnimation::render_order);
			for (it = ents.begin(); it != ents.end(); it++)
			{
				Entity &ent = *(*it);
				g->DrawImage(ent.get_entity_image(), (int)ent.get_position().x, (int)ent.get_position().y);
			}
		}*/

	for (it = eco_system->update_list.begin(); it != eco_system->update_list.end(); it++)
	{
		Entity &ent = *(*it);
		g->DrawImage(ent.get_entity_image(), (int)ent.get_position().x, (int)ent.get_position().y);
	}

	eco_system->environment->render_environment_effects(g);//�����Ⱦ����������Ч��

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
	eco_system_window = new AnimationDisplayWindow(L"EcoSystem", DEFAULT_WIDTH + 35, DEFAULT_HEIGHT + 58, eco_system_animation);
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

void EcoSystem::on_tick()//����������������̬ϵͳ��on_tick����
{
	mtx.lock();//������Ⱦ����һ���̣߳����Һ͵�ǰ�߳��й�����Դ������Ҫ�ӻ�������
	environment->on_tick();//���»���
	std::vector<Entity*>::iterator it;
	std::list<Entity*>::iterator list_it;

	//=========================��������====================================
	for (list_it = update_list.begin(); list_it != update_list.end();)
	{
		if ((*list_it)->is_alive())
		{
			Vector2D prev_pos = (*list_it)->get_position();
			int prev_chunk_r = (int)((*list_it)->get_position().x / CHUNK_SIZE);
			int prev_chunk_c = (int)((*list_it)->get_position().y / CHUNK_SIZE);
			chunk[prev_chunk_r][prev_chunk_c].position_sum[(*list_it)->get_species_name()] -= prev_pos;
			(*list_it)->on_tick();
			prevent_overstep(*list_it);
			int current_chunk_r = (int)((*list_it)->get_position().x / CHUNK_SIZE);
			int current_chunk_c = (int)((*list_it)->get_position().y / CHUNK_SIZE);
			chunk[current_chunk_r][current_chunk_c].position_sum[(*list_it)->get_species_name()] += (*list_it)->get_position();
			//if ((*list_it)->get_energy() < 0.0)
			//	(*list_it)->set_dead();

			if (prev_chunk_r != current_chunk_r || prev_chunk_c != current_chunk_c)
			{
				chunk[prev_chunk_r][prev_chunk_c].entities.erase(*list_it);
				chunk[prev_chunk_r][prev_chunk_c].count[(*list_it)->get_species_name()]--;
				
				chunk[current_chunk_r][current_chunk_c].entities.insert(*list_it);
				chunk[current_chunk_r][current_chunk_c].count[(*list_it)->get_species_name()]++;
			}

			++list_it;
		}
		else
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

	//=========================�����������===================================


	//============������ܷ������ƶ���Ҫ�����������ڵĿ�========================
	/*for (int r = 0; r < (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE; r++)
	{
		for (int c = 0; c < (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE; c++)
		{
			std::set<Entity*> &ents = entities[r][c];
			for (it = ents.begin(); it != ents.end();)
			{
				Entity &ent = *(*it);
				if (!ent.is_alive())
				{
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
	}*/
	//=============================================================================================
	mtx.unlock();
}

void EcoSystem::spawn_entity(Entity *entity, Vector2D position)//���ض�λ��position��������entity
{
	prevent_overstep(position);
	entity->set_position(position);
	int chunk_r = (int)entity->get_position().x / CHUNK_SIZE, chunk_c = (int)entity->get_position().y / CHUNK_SIZE;
	//entities[chunk_r][chunk_c].push_back(entity);
	
	chunk[chunk_r][chunk_c].entities.insert(entity);
	chunk[chunk_r][chunk_c].position_sum[entity->get_species_name()] += entity->get_position();
	chunk[chunk_r][chunk_c].count[entity->get_species_name()]++;

	update_list.push_back(entity);
}

void EcoSystem::spawn_entity(Entity *entity)//�����λ����������entity
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


bool EcoSystem::try_eat(Entity *predator, Entity *prey)//���Բ�ʳ
{
	const static double MIN_EATABLE_DISTANCE = 10.0;//���������ľ���С��10���ܲ�ʳ
	if ((food_web->is_prey(prey, predator)) //prey�Ƿ���predator��ʳ��
		&& ((predator->get_position() - prey->get_position()).modulus() < MIN_EATABLE_DISTANCE) //�����Ƿ�С��10
		&& (predator->is_alive())
		&& (prey->is_alive()))//���������Ƿ����
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

Entity *EcoSystem::find_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c)//��һ���ض����в���������type�������
{
	//��ֹԽ��
	if (chunk_r < 0 || chunk_c < 0 || chunk_r >= (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE || chunk_c >= (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE)
		return NULL;

	std::set<Entity*> &ents = chunk[chunk_r][chunk_c].entities;
	std::set<Entity*>::iterator it;
	std::vector<Entity*> entity_founded;
	/*for (it = ents.begin(); it != ents.end(); it++)
	{
		if (types.find((*it)->get_species_name()) != types.end())
			return (*it);
			//entity_founded.push_back(*it);
	}
	return NULL;*/
	
	//�ѷ����������������entity_founded
	for (it = ents.begin(); it != ents.end(); it++)
	{
		if (types.find((*it)->get_species_name()) != types.end())
			entity_founded.push_back(*it);
	}
	if (entity_founded.empty())
		return NULL;
	else
		return entity_founded[(unsigned int)(EcoSystem::random_double() * entity_founded.size())];//�������һ��
		
}

/*std::set<Entity*> EcoSystem::find_all_entity_in_chunk(std::set<std::string> &types, int chunk_r, int chunk_c)
{
	std::set<Entity*> tmp;
	if (chunk_r < 0 || chunk_c < 0 || chunk_r >= (DEFAULT_WIDTH + CHUNK_SIZE) / CHUNK_SIZE || chunk_c >= (DEFAULT_HEIGHT + CHUNK_SIZE) / CHUNK_SIZE)
		return tmp;
	std::vector<Entity*> &ents = entities[chunk_r][chunk_c];
	std::vector<Entity*>::iterator it;
	for (it = ents.begin(); it != ents.end(); it++)
	{
		if (types.find((*it)->get_species_name()) != types.end())
			tmp.insert(*it);
	}
	return tmp;
}
*/

int EcoSystem::get_chunk_entity_count(int chunk_r, int chunk_c, std::set<std::string> &types)
{
	std::set<std::string>::iterator it;
	int sum = 0;
	for (it = types.begin(); it != types.end(); it++)
		sum += chunk[chunk_r][chunk_c].count[*it];
	return sum;
}
Vector2D EcoSystem::get_chunk_entity_position_sum(int chunk_r, int chunk_c, std::set<std::string> &types)
{
	std::set<std::string>::iterator it;
	Vector2D sum;
	for (it = types.begin(); it != types.end(); it++)
		sum += chunk[chunk_r][chunk_c].position_sum[*it];
	return sum;
}
Entity *EcoSystem::find_entity(Entity *source, std::set<std::string> &types)
{
	int source_chunk_r = (int)source->get_position().x / CHUNK_SIZE, source_chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	Entity *tmp;

	//���Ե�ǰ�������ڵĿ�Ϊ���ĵ�5*5�Ŀ��в��������Ĳ���˳��Ϊ�ӽ���Զ
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

/*std::set<Entity*> EcoSystem::find_all_entity(Entity *source, std::set<std::string> &types)
{
	int source_chunk_r = (int)source->get_position().x / CHUNK_SIZE, source_chunk_c = (int)source->get_position().y / CHUNK_SIZE;
	std::set<Entity*> tmp[26];
	tmp[0]  = find_all_entity_in_chunk(types, source_chunk_r, source_chunk_c);
		    	   
	tmp[1]  = find_all_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c);
	tmp[2]  = find_all_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c);
	tmp[3]  = find_all_entity_in_chunk(types, source_chunk_r, source_chunk_c - 1);
	tmp[4]  = find_all_entity_in_chunk(types, source_chunk_r, source_chunk_c + 1);
		    	   
	tmp[5]  = find_all_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c - 1);
	tmp[6]  = find_all_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c + 1);
	tmp[7]  = find_all_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c - 1);
	tmp[8]  = find_all_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c + 1);
		    	   
	tmp[9]  = find_all_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c);
	tmp[10] = find_all_entity_in_chunk(types, source_chunk_r, source_chunk_c - 2);
	tmp[11] = find_all_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c);
	tmp[12] = find_all_entity_in_chunk(types, source_chunk_r, source_chunk_c + 2);
				   
	tmp[13] = find_all_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c - 1);
	tmp[14] = find_all_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c + 1);
	tmp[15] = find_all_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c - 2);
	tmp[16] = find_all_entity_in_chunk(types, source_chunk_r + 1, source_chunk_c + 2);
	tmp[17] = find_all_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c - 2);
	tmp[18] = find_all_entity_in_chunk(types, source_chunk_r - 1, source_chunk_c + 2);
	tmp[19] = find_all_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c - 1);
	tmp[20] = find_all_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c + 1);
				   
	tmp[21] = find_all_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c - 2);
	tmp[22] = find_all_entity_in_chunk(types, source_chunk_r - 2, source_chunk_c + 2);
	tmp[23] = find_all_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c - 2);
	tmp[24] = find_all_entity_in_chunk(types, source_chunk_r + 2, source_chunk_c + 2);

	for (int i = 0; i < 25; i++)
		tmp[25].insert(tmp[i].begin(), tmp[i].end());
	return tmp[25];
}*/

Entity *EcoSystem::find_prey(Entity *source)//��������source��ʳ����õ���find_entity
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