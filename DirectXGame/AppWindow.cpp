#include "AppWindow.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		/*
		 *{-0.5f,-0.5f,0.0f,  1,0,0},
		{-0.5f,0.5f,0.0f,  0,1,0},
		{ 0.5f,-0.5f,0.0f,  0,0,1},
		{ 0.5f,0.5f,0.0f,  1,1,0},
		*/

		{-0.7f,-0.7f,0.0f,  1,0,0},
		{-0.7f,-0.2f,0.0f,  0,1,0},
		{ -0.2f,-0.7f,0.0f,  0,0,1},
		{ -0.2f,-0.2f,0.0f,  1,1,0},

	};

	vertex list2[] =
	{
		{0.2f,-0.7f,0.0f,  1,0,0},
		{ 0.45f,-0.2f,0.0f,  0,1,0 },
		{ 0.7f,-0.7f,0.0f,  0,0,1},
	};

	vertex list3[] =
	{
		{-0.25f,0.2f,0.0f,  0,1,0},
		{-0.25f,0.7f,0.0f,  0,1,0},
		{ 0.25f,0.2f,0.0f,  0,1,0},
		{ 0.25f,0.7f,0.0f,  0,1,0},
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	m_vb2 = GraphicsEngine::get()->createVertexBuffer();
	m_vb3 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain",&shader_byte_code,&size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_vb2->load(list2, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_vb3->load(list3, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb2);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb2->getSizeVertex(), 0);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb3);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb3->getSizeVertex(), 0);

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}
