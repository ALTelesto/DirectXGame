#pragma once
#include <list>

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Quad.h"

using namespace std;
class AppWindow: public Window
{
public:
	AppWindow();
	~AppWindow();
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexBuffer* m_vb2;
	VertexBuffer* m_vb3;
	VertexShader* m_vs;
	PixelShader* m_ps;
	list<Quad*> quads;

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
};

