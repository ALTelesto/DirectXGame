#include "Window.h"

#include "EngineTime.h"

//Window* window = nullptr;

Window::Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_CREATE:
		{
			// Event fired when the window is created
			// collected here..
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			// .. and then stored for later lookup
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}
	case WM_SETFOCUS:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onFocus();
			break;
		}
	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}
	default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "MyWindowClass";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc))
		return false;

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MyWindowClass", "DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	if (!m_hwnd)
		return false;


	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	EngineTime::LogFrameStart();
	this->onUpdate();
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	Sleep(1);
	EngineTime::LogFrameEnd();
	return true;
}

bool Window::release()
{
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

Window::~Window()
{
}
