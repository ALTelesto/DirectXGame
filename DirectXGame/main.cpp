#include "AppWindow.h"
#include "EngineTime.h"

int main()
{
	AppWindow::initialize();
	InputSystem::initialize();
	AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
	runningApp->init();
	runningApp->createGraphicsWindow();

	while(runningApp->isRun())
	{
		runningApp->broadcast();
	}

	AppWindow::destroy();
	InputSystem::destroy();

	return 0;
}
