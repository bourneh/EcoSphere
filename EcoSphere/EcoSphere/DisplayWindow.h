#include <string>
#include <Windows.h>

#ifndef DS_ECOSPHERE_DISPLAYWINDOW
#define DS_ECOSPHERE_DISPLAYWINDOW

//class MouseHandler;
//class KeyboardHandler;

class DisplayWindow
{
public:
	DisplayWindow(unsigned int width, unsigned int height);
	virtual ~DisplayWindow();

	static void ThreadFunction(DisplayWindow* display_window);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void display();
	HWND get_window_handle() const;

private:
	virtual void on_quit();
	virtual void init(unsigned int width, unsigned int height);

	bool running;
	HWND hwnd;
	HINSTANCE hinst;
	std::wstring title, window_class;
	unsigned int width, height;
};

#endif