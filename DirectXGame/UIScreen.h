#pragma once
#include <string>
#include "imgui.h"

class UIManager;

using std::string;

class UIScreen
{
public:
	UIScreen(std::string name) : name(name){}

protected:
	string getName() { return name; }
	virtual void draw() = 0;

	string name;

	friend class UIManager;
};
