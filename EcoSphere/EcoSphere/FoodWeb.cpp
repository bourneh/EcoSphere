#include "FoodWeb.h"
#include "Consumer.h"
#include "Producer.h"
#include <algorithm>
#include <fstream>
using namespace std;

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

set<string> &FoodWeb::get_prey_set(Entity *source)
{
	return predator_prey_web[source->get_species_name()];
}
void FoodWeb::load_food_web_from_istream(std::istream &is)
{
	string predator, prey;
	while (is >> predator && predator != "end")
	{
		is >> prey;
		map<string, set<string> >::iterator it = predator_prey_web.find(predator);
		if (it == predator_prey_web.end())
		{
			set<string> s;
			s.insert(prey);
			predator_prey_web.insert(pair<string, set<string> >(predator, s));
		}
		else
			it->second.insert(prey);

		map<string, set<string> >::iterator it2 = prey_predator_web.find(predator);
		if (it == prey_predator_web.end())
		{
			set<string> s;
			s.insert(predator);
			predator_prey_web.insert(pair<string, set<string> >(prey, s));
		}
		else
			it->second.insert(predator);
	}
}

void FoodWeb::food_web_from_file(string file_name)
{
	ifstream ifs(file_name.c_str());
	if (!ifs.good())
	{
		cerr << "Can not open file " << file_name << ".\n";
		return;
	}

	load_food_web_from_istream(ifs);
}

void FoodWeb::food_web_from_input()
{
	load_food_web_from_istream(cin);
}




