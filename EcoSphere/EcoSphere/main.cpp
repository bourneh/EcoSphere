#include <iostream>
#include <thread>
#include <string>
#include "DisplayWindow.h"
#include "Timer.h"
#include "Vector2D.h"
#include "Animation.h"
#include <iostream>
using namespace std;
int main()
{
	DisplayWindow d(1200, 700);
	d.display();
	int a;
	cin >> a;
	return 0;
}