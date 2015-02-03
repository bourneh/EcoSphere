#include "DisplayWindow.h"
#include <iostream>
#include <map>
#include <thread>
#include <comdef.h>
#include <gdiplus.h>

std::map<unsigned int, DisplayWindow*> handle_pointer_map;
DisplayWindow::DisplayWindow(std::wstring title, unsigned int width, unsigned int height)
{
	init(title, width, height);
}

DisplayWindow::~DisplayWindow()
{
	PostMessage(hwnd, WM_DESTROY, 0, 0);
	CloseHandle(hwnd);
}

void DisplayWindow::create()
{
	std::cout << "Window created\n";
	if (!created)
	{
		std::thread t(DisplayWindow::ThreadFunction, this);
		t.detach();
		created = true;
		Sleep(10);
		handle_pointer_map[(unsigned int)hwnd] = this;
	}
}

void DisplayWindow::show()
{
	if (created)
		ShowWindow(hwnd, SW_SHOW);
	else
		std::cerr << "Not created\n";
}

void DisplayWindow::hide()
{
	if (created)
		ShowWindow(hwnd, SW_HIDE);
	else
		std::cerr << "Not created\n";
}

HWND DisplayWindow::getWindowHandle() const
{
	return hwnd;
}

std::wstring DisplayWindow::getTitle() const
{
	return title;
}

void DisplayWindow::setTitle(std::wstring title)
{
	this->title = title;
	SetWindowText(hwnd, title.c_str());
}

void DisplayWindow::on_quit()
{
	std::cout << "Display window closed" << std::endl;
	CloseHandle(hwnd);
}

void DisplayWindow::on_paint()
{

}

void DisplayWindow::init(std::wstring title, unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->window_class = L"DisplayWindow";
	created = false;
	hwnd = NULL;
}

LRESULT CALLBACK DisplayWindow::WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_PAINT:
		handle_pointer_map[(unsigned int)hWnd]->on_paint();
		break;
	default:
		return DefWindowProc(hWnd, message, wparam, lparam);
	}
	return 0;
}

void DisplayWindow::ThreadFunction(DisplayWindow* p)
{
	using namespace std;
	MSG msg;
	p->hinst = GetModuleHandle(0);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = p->hinst;
	wcex.hIcon = LoadIcon(p->hinst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = p->window_class.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	p->hwnd = CreateWindow(p->window_class.c_str(), p->title.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, p->width, p->height, NULL, NULL, p->hinst, NULL);

	if (!p->hwnd)
	{
		std::cerr << "Create display window failed with error code " << GetLastError() << ".\n";
	}
	else
	{
		std::cout << "Display window created.[Width:" << p->width << " Height:" << p->height << "]\n";
	}

	ShowWindow(p->hwnd, SW_HIDE);
	UpdateWindow(p->hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	p->on_quit();
	handle_pointer_map.erase((unsigned int)(p->getWindowHandle()));
}