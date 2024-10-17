#pragma once
#include "InputListener.h"
#include <unordered_set>

class InputSystem
{
public:
	static InputSystem* getInstance();
	static void initialize();
	static void destroy();
public:
	InputSystem();
	~InputSystem();

	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

private:
	InputSystem(InputSystem const&) {};
	InputSystem& operator = (InputSystem const&) {};
	static InputSystem* sharedInstance;
private:
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = true;
};