#include "GraphicsEngine.h"
#include "EngineTime.h"
#include <exception>

#include "LogUtils.h"

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
		throw std::exception("RenderSystem not created successfully");
	}

	try
	{
		m_mesh_manager = new MeshManager();
	}
	catch (...) { throw std::exception("MeshManager not created successfully"); }

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
	return m_render_system;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}


