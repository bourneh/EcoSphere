#include <iostream>
#include "EcoSystem.h"
#include "Demotiger.h"
#include "Environment.h"
#include "DemoCow.h"
#include "DemoGrass.h"
using namespace std;

Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR	 gdiplusToken;


int main()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	EcoSystem ecosystem;
	for (int i = 0; i < 5000; i++)
	{
		Entity *e1 = new DemoGrass(&ecosystem);
		ecosystem.spawn_entity(e1);
	}
	for (int i = 0; i < 1000; i++)
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
	be.set_default_producer(new DemoGrass(&ecosystem));
	ecosystem.set_environment(&be);

	ecosystem.run();

	int ss;
	while (cin >> ss)
	{
		for (int i = 0; i < ss; i++)
		{
			Entity *e2 = new DemoCow(&ecosystem);
			ecosystem.spawn_entity(e2);
		}
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}