#include "Animation.h"
#include <iostream>


Animation::AnimationTimerTask::AnimationTimerTask(Animation *animation)
{
	this->animation = animation;
}

void Animation::AnimationTimerTask::task()//timertask的task函数。调用Animation的render函数。
{
	animation->render(animation->g);
}

Animation::Animation(unsigned int width, unsigned int height) :
width(width), height(height)
{
	buffer = new Gdiplus::Bitmap(width, height);//缓冲区
	g = new Gdiplus::Graphics(buffer);//绘图器
	att = new AnimationTimerTask(this);
	timer = new Timer(80, *att);//创建定时器
}

Animation::~Animation()
{
	delete timer;
	delete att;
	delete g;
	delete buffer;
}

void Animation::render(Gdiplus::Graphics *g) //在派生类中重载此函数
{}

void Animation::start()
{
	timer->start();
}

void Animation::stop()
{
	timer->stop();
}

bool Animation::try_lock()
{
	return rendering_mutex.try_lock();
}

void Animation::unlock()
{
	rendering_mutex.unlock();
}

Gdiplus::Image *Animation::get_buffer_pointer()//获得缓冲区指针
{
	return buffer;
}