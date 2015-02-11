#include "FoodWeb.h"
#include "Consumer.h"
#include "Producer.h"
#include <algorithm>
#include <fstream>
using namespace std;

FoodWeb::FoodWeb(EcoSystem *eco_system) : 
EcoSystemObject(eco_system)
{}
bool FoodWeb::is_competitive(Entity *a, Entity *b)
{
	set<string> out;
	map<string, set<string> >::iterator it1, it2;
	it1 = predator_prey_web.find(a->get_species_name());
	it2 = predator_prey_web.find(b->get_species_name());
	set<string> &set_a = it1->second, &set_b = it2->second;
	set_intersection(set_a.begin(), set_a.end(), set_b.begin(), set_b.end(), inserter(out, out.begin()));
	return out.empty();
}

bool FoodWeb::is_prey(Entity *a, Entity *b)
{
	map<string, set<string> >::iterator it = predator_prey_web.find(b->get_species_name());
	set<string> &s = it->second;
	return (s.find(a->get_species_name()) != s.end());
}

bool FoodWeb::is_valid()
{
	return false;
}

void FoodWeb::add(string predator, string prey)
{
	predator_prey_web[predator].insert(prey);
	prey_predator_web[prey].insert(predator);
}

set<string> &FoodWeb::get_prey_set(Entity *source)
{
	return predator_prey_web[source->get_species_name()];
}

std::set<std::string> &FoodWeb::get_predator_set(Entity *source)
{
	return prey_predator_web[source->get_species_name()];
}
