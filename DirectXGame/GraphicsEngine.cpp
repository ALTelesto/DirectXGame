#include "GraphicsEngine.h"
#include "EngineTime.h"
#include <exception>

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine* GraphicsEngine::getInstance()
{
	return sharedInstance;
}

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("GraphicsEngine not created successfully");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_render_system;
}

void GraphicsEngine::initialize()
{
	if (GraphicsEngine::sharedInstance) throw std::exception("Graphics Engine already initialized");
	sharedInstance = new GraphicsEngine();
}

void GraphicsEngine::destroy()
{
	if(sharedInstance != NULL)
	{
		delete sharedInstance;
	}
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}


