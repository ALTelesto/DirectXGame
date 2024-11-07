#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"
#include "WICTextureLoader.h"
#include "DirectXHelpers.h"
#include "Prerequisites.h"

#include "UIScreen.h"

#include "MenuScreen.h"

class UIManager
{
	typedef std::shared_ptr<UIScreen> UIScreenPtr;
	typedef std::vector<UIScreenPtr> UIList;
public:
	static UIManager* getInstance();
	static void initialize(HWND windowHandle);
	static void destroy();

	void draw() const;

private:
	bool about_active = false;
private:
	UIManager(HWND windowHandle);
	~UIManager();
	UIManager(UIManager const&) {};
	UIManager& operator = (UIManager const&) {};
	static UIManager* sharedInstance;

	UIList uiList;
};

