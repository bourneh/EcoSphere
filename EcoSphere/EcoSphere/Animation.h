#include <windows.h>
#include <gdiplus.h>
#include "Timer.h"

#ifndef DS_ECOSYSTEM_ANIMATION
#define DS_ECOSYSTEM_ANIMATION

class RenderTask
{
public:
	virtual Gdiplus::Image* render();
	virtual ~RenderTask();
	Gdiplus::Graphics* get_graphics_instance();
protected:
	RenderTask(unsigned int width, unsigned int height);
private:
	Gdiplus::Bitmap *buffer;
	Gdiplus::Graphics *g;
};

class Animation
{
	class AnimationTimerTask : public TimerTask
	{
	public:
		AnimationTimerTask(HWND hWnd, RenderTask &render_task);
		void task();
	private:
		HWND hWnd;
		RenderTask &render_task;
		Gdiplus::Graphics g;
	};

public:
	Animation(HWND hWNd, double fps, RenderTask &render_task);
	virtual ~Animation();
	void start();
	void stop();
	bool is_running() const;

private:
	RenderTask &render_task;
	AnimationTimerTask animate;
	Timer timer;
};

#endif
