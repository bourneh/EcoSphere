#include <windows.h>
#include <thread>
#include "Timer.h"

using namespace std;

Timer::Timer(unsigned int period, TimerTask &task) :
task(task)
{
	this->period = period;
	running = false;
}

Timer::~Timer()
{
	stop();
}

void Timer::threadfunction(Timer *timer)
{
	unsigned int start_time = GetTickCount();
	while (timer->running)
	{
		unsigned int current_time = GetTickCount();
		if ((current_time - start_time) / timer->period > 0)
		{
			start_time = current_time;
			timer->task.task();
		}
		Sleep((timer->period / 10) > 0 ? (timer->period / 10) : 1);
	}
}

void Timer::start()
{
	if (running)
		return;
	running = true;
	std::thread t(threadfunction, this);
	t.detach();
}

void Timer::stop()
{
	running = false;
}

bool Timer::is_running() const
{
	return running;
}

