#include "AppWindow.h"
#include "EngineTime.h"

int main()
{
	try
	{
		GraphicsEngine::initialize();
		InputSystem::initialize();
	}
	catch (...) { return -1; }

	{
		AppWindow::initialize();
		//InputSystem::initialize();
		AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
		runningApp->init();
		runningApp->createGraphicsWindow();

		while(runningApp->isRun())
		{
			runningApp->broadcast();
		}

		AppWindow::destroy();
	}

	InputSystem::destroy();
	GraphicsEngine::destroy();

	return 0;
}
