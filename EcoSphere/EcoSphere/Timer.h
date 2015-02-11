/*
 *定时器。每隔一段时间调用一次对应TimerTask的task函数。
 *精度大约为30ms。如果周期设置得比30ms短的话计时可能不准。
*/

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