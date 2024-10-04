#include "AppWindow.h"
#include "EngineTime.h"

int main()
{
	AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
	runningApp->init();
	runningApp->createGraphicsWindow();

	while(runningApp->isRun())
	{
		runningApp->broadcast();
	}

	return 0;
}
