#include "EcoSystemObject.h"
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#ifndef DS_ECOSYSTEM_FOODWEB
#define DS_ECOSYSTEM_FOODWEB



class EcoSystem;
class Entity;
class Environment;

class FoodWeb : public EcoSystemObject
{
public:
	FoodWeb(EcoSystem *eco_system);
	bool is_competitive(Entity *a, Entity *b);
	bool is_prey(Entity *a, Entity *b);
	bool is_valid();

	std::set<std::string> &get_prey_set(Entity *source);
	std::set<std::string> &get_predator_set(Entity *source);
	void add(std::string predator, std::string prey);
private:
	std::map<std::string, std::set<std::string> > predator_prey_web, prey_predator_web;
};

#endif // DS_ECOSYSTEM_FOODWEB
