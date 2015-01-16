#include "Animation.h"
#include <iostream>
Animation::AnimationTimerTask::AnimationTimerTask(HWND hWnd, RenderTask &render_task) :
hWnd(hWnd), render_task(render_task), g(hWnd) {}

void Animation::AnimationTimerTask::task()
{
	Gdiplus::Color c(255, 255, 255);
	render_task.get_graphics_instance()->Clear(c);
	g.DrawImage(render_task.render(), 0, 0);
}

Animation::Animation(HWND hWnd, double fps, RenderTask &render_task) :
render_task(render_task), animate(hWnd, render_task), timer(1000 / fps, animate) {}

Animation::~Animation() {}

void Animation::start()
{
	timer.start();
}

void Animation::stop()
{
	timer.stop();
}

bool Animation::is_running() const
{
	return timer.is_running();
}
RenderTask::RenderTask(unsigned int width, unsigned int height)
{
	buffer = new Gdiplus::Bitmap(width, height);
	g = new Gdiplus::Graphics(buffer);
}

RenderTask::~RenderTask()
{
	delete buffer;
	delete g;
}

Gdiplus::Image* RenderTask::render()
{
	return buffer;
}

Gdiplus::Graphics* RenderTask::get_graphics_instance()
{
	return g;
}
