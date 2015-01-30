#include <iostream>
#include "EcoSystem.h"
#include "Demotiger.h"
#include "Environment.h"
#include "DemoCow.h"
#include "Grass.h"
#include "FoodWeb.h"
using namespace std;

Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR	 gdiplusToken;


int main()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	EcoSystem ecosystem;
	FoodWeb fw(&ecosystem);
	ecosystem.set_food_web(&fw);
	fw.add("hehetiger", "hehecow");
	fw.add("hehecow", "Grass");
	/*for (int i = 0; i < 10000; i++)
	{
		Entity *e1 = new Grass(&ecosystem);
		ecosystem.spawn_entity(e1);
	}*/
	for (int i = 0; i < 100; i++)
	{
		Entity *e2 = new DemoCow(&ecosystem);
		ecosystem.spawn_entity(e2);
	}

	for (int i = 0; i < 5; i++)
	{
		Entity *e2 = new DemoTiger(&ecosystem);
		ecosystem.spawn_entity(e2);
	}


	Environment be(&ecosystem);
	be.set_default_producer(new Grass(&ecosystem));
	ecosystem.set_environment(&be);

	ecosystem.run();

	int ss;
	while (cin >> ss)
	{
		be.set_temperature(ss);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}