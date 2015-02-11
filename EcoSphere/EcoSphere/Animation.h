#include <windows.h>
#include <gdiplus.h>
#include <mutex>
#include "Timer.h"
/*===========================================================================
 *Animation�࣬������ʾ����Ч������Timer������ÿ��timer�¼�����һ��render������
 *����ͨ������render����ʵ���Զ��嶯��Ч����
 *render�������ÿ֡���滭��һ��buffer����ڰ�buffer������ʾ������ʾ������
 *���ڿ�������ʾ�����Ĵ��ڲ���ͬһ�̣߳����Դ���һ����������
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
