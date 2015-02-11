#include <string>
#include <map>
#include <Windows.h>


/*
 *´°¿Ú
*/
#ifndef DS_ECOSPHERE_DISPLAYWINDOW
#define DS_ECOSPHERE_DISPLAYWINDOW

//class MouseHandler;
//class KeyboardHandler;

class DisplayWindow
{
public:
	DisplayWindow(std::wstring title = L"DisplayWindow", unsigned int width = 400, unsigned int height = 300);
	virtual ~DisplayWindow();

	static void ThreadFunction(DisplayWindow* display_window);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND getWindowHandle() const;
	std::wstring getTitle() const;
	void setTitle(std::wstring title);
	void create();
	void show();
	void hide();
protected:
	virtual void on_paint();
	virtual void on_quit();
private:
	virtual void init(std::wstring title, unsigned int width, unsigned int height);

	bool created;
	HWND hwnd;
	HINSTANCE hinst;
	std::wstring title, window_class;
	unsigned int width, height;

};

#endif