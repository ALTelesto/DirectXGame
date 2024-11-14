#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "MeshManager.h"

class GraphicsEngine
{
public:
	static GraphicsEngine* getInstance();
	static void initialize();
	static void destroy();

	RenderSystem* getRenderSystem();
	MeshManager* getMeshManager();
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);
private:
	GraphicsEngine();
	~GraphicsEngine();
	GraphicsEngine(GraphicsEngine const&) {};
	GraphicsEngine& operator = (GraphicsEngine const&) {};
	static GraphicsEngine* sharedInstance;
private:
	RenderSystem* m_render_system = nullptr;
	MeshManager* m_mesh_manager = nullptr;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};

