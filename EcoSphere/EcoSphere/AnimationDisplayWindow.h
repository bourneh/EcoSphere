#include "DisplayWindow.h"
#include "Animation.h"
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