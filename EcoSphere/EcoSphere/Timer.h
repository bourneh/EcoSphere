#ifndef DS_ECOSYSTEM_TIMER
#define DS_ECOSYSTEM_TIMER

class TimerTask
{
public:
	virtual void task() = 0;
};

class Timer
{
public:
	Timer(unsigned int period, TimerTask &task);
	virtual ~Timer();
	static void threadfunction(Timer *timer);
	void start();
	void stop();
	bool is_running() const;
private:
	unsigned int period;
	bool running;
	TimerTask &task;
};


#endif