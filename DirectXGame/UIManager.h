#pragma once
#include <Windows.h>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Prerequisites.h"

class UIManager
{
public:
	static UIManager* getInstance();
	static void initialize(HWND windowHandle);
	static void destroy();

	void drawAllUI();
private:
	UIManager(HWND windowHandle);
	~UIManager();
	UIManager(UIManager const&) {};
	UIManager& operator = (UIManager const&) {};
	static UIManager* sharedInstance;
};

