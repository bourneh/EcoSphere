#include "DisplayWindow.h"
#include "Animation.h"
/*
 *用于显示动画的窗口。可以通过重载on_paint把Animation类里的buffer画在窗口上显示动画。 
*/

#ifndef DS_ECOSYSTEM_ANIMATIONDISPLAYWINDOW
#define DS_ECOSYSTEM_ANIMATIONDISPLAYWINDOW

class AnimationDisplayWindow : public DisplayWindow
{
public:
	AnimationDisplayWindow(std::wstring title, unsigned int width, unsigned int height, Animation *animation);
	virtual ~AnimationDisplayWindow();
	void start();
	void stop();
protected:
	virtual void on_quit();
	virtual void on_paint();
private:
	Animation *animation;
	Gdiplus::Graphics *g;
	bool running;
};

#endif