#include <windows.h>
#include <gdiplus.h>
#include <mutex>
#include "Timer.h"


/* Animation class
 * Functionality: To display animations
 * Triggered by a Timer
 * Can display self-customized animations by overload the render() function
 *
 */


#ifndef DS_ECOSYSTEM_ANIMATION
#define DS_ECOSYSTEM_ANIMATION

class Animation
{
	friend class AnimationTimerTask;

	//Timertask of the timer that triggers animation 
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

	//Get the pointer of the buffer
	Gdiplus::Image *get_buffer_pointer();

	//Render one frame
	virtual void render(Gdiplus::Graphics *g);

	//Start animation
	void start();

	//Stop animation
	void stop();

	//lock the mutex
	bool try_lock();

	//unlock the mutex
	void unlock();

protected:
	//Width and height of the buffer canvas
	unsigned int width, height;
	//Mutex for multithread rendering
	std::mutex rendering_mutex;
private:
	AnimationTimerTask *att;
	Gdiplus::Image *buffer;//The buffer canvas
	Gdiplus::Graphics *g;
	Timer *timer;
};
#endif
