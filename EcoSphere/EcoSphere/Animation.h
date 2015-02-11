#include <windows.h>
#include <gdiplus.h>
#include <mutex>
#include "Timer.h"
/*===========================================================================
 *Animation类，用于显示动画效果，由Timer触发，每个timer事件调用一次render函数。
 *可以通过重载render函数实现自定义动画效果。
 *render函数会把每帧画面画在一个buffer里，窗口把buffer画在显示区来显示动画。
 *由于可能与显示动画的窗口不在同一线程，所以带了一个互斥锁。
 */
#ifndef DS_ECOSYSTEM_ANIMATION
#define DS_ECOSYSTEM_ANIMATION

class Animation
{
	friend class AnimationTimerTask;
	class AnimationTimerTask : public TimerTask
	{
	public:
		AnimationTimerTask(Animation *animation);
		void task();
	private:
		Animation *animation;
	};
public:
	Animation(unsigned int width, unsigned int height);
	virtual ~Animation();
	Gdiplus::Image *get_buffer_pointer();
	virtual void render(Gdiplus::Graphics *g);
	void start();
	void stop();
	bool try_lock();
	void unlock();
protected:
	unsigned int width, height;
	std::mutex rendering_mutex;
private:
	AnimationTimerTask *att;
	Gdiplus::Image *buffer;
	Gdiplus::Graphics *g;
	Timer *timer;
};
#endif
