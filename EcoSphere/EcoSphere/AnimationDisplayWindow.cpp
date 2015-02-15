#include "AnimationDisplayWindow.h"

AnimationDisplayWindow::AnimationDisplayWindow(std::wstring title, unsigned int width, unsigned int height, Animation *animation) :
DisplayWindow(title, width, height)
{
	create();
	this->animation = animation;
	g = new Gdiplus::Graphics(getWindowHandle());
	running = false;
}
AnimationDisplayWindow::~AnimationDisplayWindow()
{
	on_quit();
}
void AnimationDisplayWindow::start()
{
	running = true;
	animation->start();
}

void AnimationDisplayWindow::stop()
{
	animation->stop();
}

void AnimationDisplayWindow::on_quit()
{
	delete animation;
	delete g;
	DisplayWindow::on_quit();
}
void AnimationDisplayWindow::on_paint()
{
	if (!running)
	{
		g->Clear(Gdiplus::Color(255, 255, 255));
	}
	else if (animation->try_lock())
	{
		//Draw the image on the buffer canvas onto the client area of the window
		g->DrawImage(animation->get_buffer_pointer(), 0, 0);
		animation->unlock();
	}
}