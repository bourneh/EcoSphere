#include "Animation.h"
#include <iostream>


Animation::AnimationTimerTask::AnimationTimerTask(Animation *animation)
{
	this->animation = animation;
}

void Animation::AnimationTimerTask::task()//timertask��task����������Animation��render������
{
	animation->render(animation->g);
}

Animation::Animation(unsigned int width, unsigned int height) :
width(width), height(height)
{
	buffer = new Gdiplus::Bitmap(width, height);//������
	g = new Gdiplus::Graphics(buffer);//��ͼ��
	att = new AnimationTimerTask(this);
	timer = new Timer(80, *att);//������ʱ��
}

Animation::~Animation()
{
	delete timer;
	delete att;
	delete g;
	delete buffer;
}

void Animation::render(Gdiplus::Graphics *g) //�������������ش˺���
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

Gdiplus::Image *Animation::get_buffer_pointer()//��û�����ָ��
{
	return buffer;
}