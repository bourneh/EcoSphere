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
	bool is_competitive(Entity *a, Entity *b);
	bool is_prey(Entity *a, Entity *b);
	bool is_valid();

	std::set<std::string> &get_prey_set(Entity *source);
	void food_web_from_file(std::string file_name);
	void food_web_from_input();
	void load_food_web_from_istream(std::istream &is);
private:
	std::map<std::string, std::set<std::string> > predator_prey_web, prey_predator_web;
};

#endif // DS_ECOSYSTEM_FOODWEB
